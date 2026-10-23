import serial
import struct
import time

# --- Setup Configuration Parameters ---
COM_PORT = 'COM5'   # Change this to match your real Device Manager COM port number
BAUD_RATE = 115200  # Matches the Blue Pill internal clock divider math
FRAME_SIZE = 64      # Strict step execution width (Processes data 4 bytes at a time)

def calculate_hardware_crc(payload_bytes):
    crc = 0xFFFFFFFF
    for byte in payload_bytes:  
        # XOR the incoming byte into the uppermost 8 bits (Bit 31 to 24)
        crc ^= (byte << 24)     
        # Execute exactly 8 shifts for this byte
        for _ in range(8):      
            if crc & 0x80000000:
                crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF                
            else:
                crc = (crc << 1) & 0xFFFFFFFF
    return crc

def send_frame(ser, data):
    """
    Constructs and transmits a uniform sequential protocol packet frame:
    [SOF (0x02)] [Length] [4-Byte Data Payload] [4-Byte CRC] [EOF (0x03)]
    """
    sof = b'\x02'
    eof = b'\x03'
    length = bytes([len(data)])
    
    # Calculate the hardware matching CRC value 
    crc_val = calculate_hardware_crc(data)
    crc_bytes = struct.pack('>I', crc_val)  # Pack integer into 4 bytes (Big-Endian)
    
    # Combine framing fields, the data payload chunk, and the signature CRC into one packet
    frame = sof + length + data + crc_bytes + eof
    ser.write(frame)

def main():
    print(f"Opening connection on {COM_PORT}...")
    try:
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=2)
    except Exception as e:
        print(f"Error opening port: {e}")
        print("Please check that the device is connected and you have closed other terminal apps.")
        return

    # Read the compiled target application executable binary file
    try:
        with open("app.bin", "rb") as f:
            binary_data = f.read()
    except FileNotFoundError:
        print("Error: 'app.bin' not found. Please place your binary file in the script directory.")
        ser.close()
        return

    total_len = len(binary_data)
    offset = 0
    frame_count = 0

    # Give the STM32 Bootloader a brief window to complete its initial power-on page erase
    print("Waiting for bootloader initialization window...")
    time.sleep(1.0)

    print(f"Sending binary file ({total_len} bytes) in strict {FRAME_SIZE}-byte block frames...")

    # Loop through the binary data and slice it into 4-byte steps
    while offset < total_len:
        # 1. Extract exactly 4 bytes from the file array buffer
        raw_chunk = binary_data[offset:offset+FRAME_SIZE]
        actual_bytes_read = len(raw_chunk) 

        # 2. Build mutable byte array payload copy
        chunk_to_send = bytearray(raw_chunk)
        
        # إرسال الإطار الحالي
        send_frame(ser, bytes(chunk_to_send))
        print(f"Sent Frame {frame_count} ({len(chunk_to_send)} bytes payload)... Waiting for ACK.")
            
        # Read single-byte acknowledgment response from STM32 Bootloader
        response = ser.read(1)
            
        if response == b'A':
            print(f"Frame {frame_count} ACK received.")
            offset += actual_bytes_read  
            frame_count += 1
        elif response == b'M':
            print(f"Frame {frame_count} NACK M received! (SOF Error) Retrying...")
            time.sleep(0.1)  
        elif response == b'N':
            print(f"Frame {frame_count} NACK N received! (EOF Error) Retrying...")
            time.sleep(0.1)  
        elif response == b'O':
            print(f"Frame {frame_count} NACK O received! (CRC Mismatch Error) Retrying...")
            time.sleep(0.1)  
        else:
            print("Timeout or unknown response from STM32. Retrying...")
            time.sleep(0.5)

    # Transmission loop complete; Send an empty Finish Frame (length parameter byte = 0)
    while True:
        print("Sending Finish Frame...")
        send_frame(ser, b"")
        response = ser.read(1)
        if response == b'F':
            print("Bootloading Completed Successfully! User Application is now running.")
            break
        else:
            print("Retrying Finish Frame...")
            time.sleep(0.5)

    ser.close()
    print("Port closed.")

if __name__ == "__main__":
    main()
