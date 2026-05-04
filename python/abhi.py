import numpy as np
import matplotlib.pyplot as plt
# Define parameters
fs = 50e6 # Sampling frequency (50 MHz)
pulse_width = 1e-6 # Pulse width (1 microsecond)
pulse_freq = 1e6 # Pulse frequency (1 MHz)
PRI = 10e-6 # Pulse Repetition Interval (10 µs)
num_pulses = 3 # Number of pulses
# Generate time axis
t = np.arange(0, num_pulses * PRI, 1/fs) # Total time duration
# Generate pulse train
pulse_train = np.zeros_like(t)
for i in range(num_pulses):
 start_idx = int(i * PRI * fs)  
end_idx = start_idx + int(pulse_width * fs)  
pulse_train[start_idx:end_idx] = np.sin(2 * np.pi * pulse_freq * t[start_idx:end_idx])  
# Plot the pulse train
plt.figure(figsize=(10, 4))
plt.plot(t * 1e6, pulse_train, label="Radar Pulse Train")
plt.xlabel("Time (µs)")
plt.ylabel("Amplitude")
plt.title("Radar Pulse ")
plt.grid()
plt.legend()
plt.xlim(0, num_pulses * PRI * 1e6) # Show only the pulseduration
plt.show()