import matplotlib.pyplot as plt
import numpy as np
import serial

def compute_fft(t, y):
    n = len(y)
    k = np.arange(n)
    T = n / Fs

    frq = k / T
    frq = frq[range(int(n/2))]

    Y = np.fft.fft(y) / n
    Y = Y[range(int(n/2))]

    return frq, Y

SERIAL_PORT = '/dev/tty.usbmodem1101'
DATA_POINTS = 1000

print("Opening port:", SERIAL_PORT)
ser = serial.Serial(SERIAL_PORT, 115200, timeout=1)

ser.write(f"{DATA_POINTS}\n".encode())

index = []
times = []
raw_data = []
filtered_data = []

data_received = 0

while data_received < DATA_POINTS:
    data_str = ser.read_until(b'\n')
    # print(data_str)
    # guard against the first lines which comes back as '', all data eventually gets back though
    if not data_str:
        continue

    data_f = list(map(float, data_str.split()))

    index.append(data_f[0])
    times.append(data_f[1])
    raw_data.append(data_f[2])
    filtered_data.append(data_f[3])

    data_received += 1


# convert to numpy arrays
t = np.array(times)
y_raw = np.array(raw_data)
y_filt = np.array(filtered_data)

# estimate sampling frequency
Fs = len(t) / ((t[-1] - t[0]) / 1000)



frq_raw, Y_raw = compute_fft(t, y_raw)
frq_filt, Y_filt = compute_fft(t, y_filt)

fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(12, 8))

# raw signal
ax1.plot(t, y_raw, 'b')
ax1.set_title("Raw Signal")
ax1.set_xlabel("Time")
ax1.set_ylabel("Amplitude")

# raw fft
ax2.loglog(frq_raw, np.abs(Y_raw), 'b')
ax2.set_title("Raw FFT")
ax2.set_xlabel("Frequency (Hz)")
ax2.set_ylabel("|Y(f)|")

# iir filtered signal
ax3.plot(t, y_filt, 'r')
ax3.set_title("Filtered Signal")
ax3.set_xlabel("Time")
ax3.set_ylabel("Amplitude")

# iir filtered fft
ax4.loglog(frq_filt, np.abs(Y_filt), 'r')
ax4.set_title("Filtered FFT")
ax4.set_xlabel("Frequency (Hz)")
ax4.set_ylabel("|Y(f)|")

plt.tight_layout()
plt.show()