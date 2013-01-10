function characterizeRadioLink(testName)
% characterizeRadioLink(testName)
%   Performs characterization of the radio link connected to the current active
%   WAVE audio interface (full-duplex mode is required since these tests perform
%   simulataneous transmit and receive). 
%
%   The specified test name is used for naming of all saved plots.
%
%   The tests performed in this suite are as follows:
%       - Noise floor of the link
%       - Frequency response of the link
%       - Frequency offset error of the link
%       - Phase noise of the link
%
%   Plots are automatically saved in the plot path specified in startup.m.

    global SAMPLE_RATE;

    characterizeNoiseFloor(SAMPLE_RATE, 1000, testName);
    characterizeFrequencyResponse(SAMPLE_RATE, 200, testName);
    characterizeFrequencyError(SAMPLE_RATE, 500, testName);
    characterizePhaseNoise(SAMPLE_RATE, 500, 1000, testName);

end


function characterizeNoiseFloor(sampleRate, durationMs, testName)
    global PLOT_PATH;
    global PLOT_EXTENSION;

    disp('=== Characterize Noise Floor ===');

    freq = 1000;
    duration = round(durationMs / 1000 * sampleRate);
    saveWidth = 0.8 * duration;

    % Plot spectrum of noise
    txSamples = tone(freq, sampleRate, duration);
    rxSamples = radioLink(txSamples, sampleRate); 
    rxSamples = trimSamples(rxSamples, saveWidth);
    rxSamples = 2 * rxSamples .* hanning(length(rxSamples))';
    noiseSpectrum = fft(rxSamples) / saveWidth;
    noiseSpectrum = [noiseSpectrum(1) 2*noiseSpectrum(2:round(end/2))];
    noiseSpectrum = noiseSpectrum .* conj(noiseSpectrum);
    freqBins = [0:length(noiseSpectrum)-1] * sampleRate / saveWidth; 
    plot(freqBins, 10*log10(noiseSpectrum))
    grid on;
    title([ ...
        sprintf('{\\bfRadio Link Tone Power Spectrum - %s}\n', testName) ...
        sprintf('Frequency = %.0f Hz, Duration = %.0f ms', freq, durationMs) ]);
    xlabel('Frequency (Hz)');
    ylabel('Power (dB)');
    axis([min(freqBins) max(freqBins) -120 0]);
    saveas(gcf, [PLOT_PATH sprintf('tone_spectrum_%s', ...
        testNameToFilename(testName)) PLOT_EXTENSION]);

    % Measurement filter (used before measuring power)
    ft = 1000 / (sampleRate / 2);
    coeff = firls(256, [0 ft ft 1], [1 1 0 0]);

    txSamples = tone(freq, sampleRate, duration);
    rxSamples = radioLink(txSamples, sampleRate); 
    rxSamples = downShift(sampleRate, rxSamples, freq);
    rxSamples = conv(coeff, rxSamples);
    rxSamples = trimSamples(rxSamples, saveWidth);
    tonePower = sumsqr(2 * abs(rxSamples)) / saveWidth;
    
    txSamples = zeros(1, duration);
    rxSamples = radioLink(txSamples, sampleRate); 
    rxSamples = downShift(sampleRate, rxSamples, freq);
    rxSamples = conv(coeff, rxSamples);
    rxSamples = trimSamples(rxSamples, saveWidth);
    noisePower = sumsqr(2 * abs(rxSamples)) / saveWidth;

    tonePower = tonePower - noisePower;
    filterNoiseBW = noisebw(coeff, 1, length(coeff), sampleRate);
    noiseDensity = noisePower / filterNoiseBW;

    disp(sprintf('Tone power = %f, Noise power = %f', tonePower, noisePower));
    disp(sprintf('SNR = %.2f dB', 10 * log10(tonePower / noisePower)));
    disp(sprintf('Filter noise bandwidth (one-sided) = %.2f Hz', filterNoiseBW / 2)); 
    disp(sprintf('Noise spectral density (one-sided) = %d', 2 * noiseDensity));
end


function characterizePhaseNoise(sampleRate, durationMs, freq, testName);
    global PLOT_PATH;
    global PLOT_EXTENSION;

    disp('=== Characterize Phase Noise ===');

    duration = round((durationMs / 1000) * sampleRate);

    txSamples = tone(freq, sampleRate, duration);
    rxSamples = radioLink(txSamples, sampleRate);

    % Obtain a frequency offset estimate
    iq = downShift(sampleRate, rxSamples, freq);
    deltas = trimSamples(diffAngles(angle(iq)), 0.6*duration);
    freqOffset = mean(deltas * sampleRate/(2*pi));

    disp(sprintf('Frequency offset = %f Hz', freqOffset));

    % Downshift again, compensating for offset
    iq = downShift(sampleRate, rxSamples, freq - freqOffset);
    deltas = trimSamples(diffAngles(angle(iq)), 0.6*duration);
    freqOffset = mean(deltas * sampleRate/(2*pi));

    disp(sprintf('Compensated frequency offset = %f Hz', freqOffset));

    deltas = deltas * 180 / pi;
    phase = cumsum(deltas);

    cdfplot(phase);
    titleText = [ ...
        sprintf('{\\bfRadio Link Phase Noise CDF - %s}\n', testName) ...
        sprintf('Frequency = %.0f Hz, Duration = %.0f ms', freq, durationMs) ];
    title(titleText);
    xlabel('Phase (Degrees)');
    saveas(gcf, [PLOT_PATH sprintf('phase_noise_cdf_%s', ...
        testNameToFilename(testName)) PLOT_EXTENSION]);
end


function characterizeFrequencyError(sampleRate, durationMs, testName)
    global PLOT_PATH;
    global PLOT_EXTENSION;

    disp('=== Characterize Frequency Error ===');

    duration = round((durationMs / 1000) * sampleRate);
    freqRange = linspace(300, 3000, 20);
    freqError = [];

    for freq = freqRange
        txSamples = tone(freq, sampleRate, duration);
        rxSamples = radioLink(txSamples, sampleRate);

        iq = downShift(sampleRate, rxSamples, freq);

        deltas = trimSamples(diffAngles(angle(iq)), 0.2*duration);
        deltas = deltas*sampleRate/(2*pi);
        freqError = [freqError mean(deltas)];
    end

    plot(freqRange, freqError);
    titleText = [...
        sprintf('{\\bfRadio Link Frequency Offset - %s}\n', testName) ...
        sprintf('Tone Duration: %.0f ms', durationMs) ];
    title(titleText);
    xlabel('Frequency (Hz)');
    ylabel('Frequency Offset (Hz)');
    saveas(gcf, [PLOT_PATH sprintf('frequency_error_%s', ...
        testNameToFilename(testName)) PLOT_EXTENSION]);
end


function deltas = diffAngles(anglesIn)
    % Differentiate a series of phase angle samples. Fix the wrap
    % around condition (when the angle completes a circle from pi to 
    % -pi or vice versa) by assuming we have wrapped whenever we have 
    % a very large delta between two successive samples. This effectively
    % limits the maximum rate of change (frequency) that can be
    % differentiated without error.
    maxDelta = pi;
    delayed = [0 anglesIn(1:end-1)];
    deltas = anglesIn - delayed;
    deltas = deltas + (deltas < -maxDelta) * (2*pi);
    deltas = deltas - (deltas > maxDelta) * (2*pi);
end


function characterizeFrequencyResponse(sampleRate, durationMs, testName)
    global PLOT_PATH;
    global PLOT_EXTENSION;

    disp('=== Characterize Frequency Response ===');

    duration = round(durationMs / 1000 * sampleRate);
    freqRange = logspace(2, 4, 100);

    % Brick wall filter (don't make the filter too narrow: this could adversely
    % affect the frequency response due to frequency offset errors over the radio link!)
    ft = 200 / (sampleRate / 2);
    filterCoeff = firls(256, [0 ft ft 1], [1 1 0 0]);

    amplitude = [];
    for freq = freqRange
        testTone = tone(freq, sampleRate, duration); 
        samples = radioLink(testTone, sampleRate);

        % Down-shift and filter     
        samples = downShift(sampleRate, samples, freq);
        samples = conv(samples, filterCoeff);
        samples = 2 * abs(samples);

        % Average middle group of amplitude samples for better noise rejection
        samples = trimSamples(samples, 0.5 * duration);
        amplitude = [amplitude 20 * log10(mean(samples))];
        disp(sprintf('Sweep %.1fHz/%.1fHz: %.1fdB', freq, max(freqRange), amplitude(end)));
    end

    % Plot and save
    semilogx(freqRange, amplitude);
    titleText = [ ...
        sprintf('{\\bfRadio Link Frequency Response - %s}\n', testName) ...
        sprintf('Maximum Amplitude = %.1f dB, Tone Duration = %.0f ms', ...
        max(amplitude), durationMs) ];
    title(titleText);
    xlabel('Frequency (Hz)');
    ylabel('Amplitude (dB)');
    grid on;
    saveas(gcf, [PLOT_PATH sprintf('frequency_response_%s', ...
        testNameToFilename(testName)) PLOT_EXTENSION]);
end
