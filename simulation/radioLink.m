function rxSamples = radioLink(txSamples, sampleRate)
% rxSamples = radioLink(txSamples, sampleRate)
%   Passes the given samples through the radio link connected to the active sound device.
%   It is assumed that the uplink is connected to the default audio playback interface
%   and the downlink is connected to the default audio recording interface. The audio 
%   sampling rate for both transmission and reception is specified by sampleRate.
%
%   Note that the received samples are not synchronized with the transmitted samples.
%   For applications where the delay is critical, a timing offset of the received
%   samples must be obtained through other means.
%
%   NOTE: For this to function properly, the AudioIf.exe tool must be in your path.

% WARNING: This must be one of the stardard PCM rates actually supported!
% (funky things happen otherwise)
pcmSampleRate = 22050;

sampleCount = length(txSamples);
pad = sampleRate / 10;
txSamples = [zeros(1, pad) txSamples];

% Convert to PCM sample rate
txSamples = resample(txSamples, pcmSampleRate, sampleRate);

% Limit samples to [-1, 1] then scale down slightly to avoid clipping 
txSamples = max(txSamples, -1 * ones(1, length(txSamples)));
txSamples = min(txSamples, ones(1, length(txSamples)));
txSamples = 0.999 * txSamples;

wavwrite(txSamples, pcmSampleRate, 16, 'wave_tx.wav');
system(['AudioIf.exe wave_tx.wav wave_rx.wav ' num2str(pcmSampleRate)]); 
rxSamples = wavread('wave_rx.wav');

% Convert from PCM sample rate
rxSamples = resample(rxSamples, sampleRate, pcmSampleRate)';
