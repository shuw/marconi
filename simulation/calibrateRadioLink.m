function calibrateRadioLink(frequency, durationMs)
% calibrateRadioLink(frequency, durationMs)
%   Use to calibrate gain settings for radio link testing. Displays the
%   FFT of the received signal as well as received tone power and THD in 
%   realtime.
%
%   A test tone of the specified frequency is used with the given 
%   duration in milliseconds.

    global SAMPLE_RATE;

    duration = round(durationMs / 1000 * SAMPLE_RATE);

    while 1
        txSamples = tone(frequency, SAMPLE_RATE, duration);
        txPower = dot(txSamples, txSamples) / duration; 

        rxSamples = radioLink(txSamples, SAMPLE_RATE);

        % Use a group of samples from the middle of the received sequence to
        % measure the results so we aren't affected by the unknown effects at
        % each edge.
        sampleCount = length(rxSamples);
        rxSamples = ...
            rxSamples(round(sampleCount/2 - (duration/4)):round(sampleCount/2 + duration/4 - 1));

        % Compute one-sided power spectrum
        powerSpectrum = fft(rxSamples) / length(rxSamples);
        powerSpectrum = powerSpectrum .* conj(powerSpectrum);
        powerSpectrum = 2 * powerSpectrum(1:round(end/2));
        binsPerHz = length(powerSpectrum) / (SAMPLE_RATE / 2);
        
        % Find the fundamental and harmonics
        fundamental = pickMaxBin(powerSpectrum, round(binsPerHz * frequency));
        harmonics = fundamental * [1:floor(length(powerSpectrum) / fundamental - 1)];
        h1 = harmonics;
        harmonics = pickMaxBin(powerSpectrum, harmonics);
        
        rxPower = powerSpectrum(harmonics(1));
        rxHarmonics = sum(powerSpectrum(harmonics(2:end)));

        % Update plot
        binCount = length(powerSpectrum); 
        freqRange = [1:binCount] * (SAMPLE_RATE/2) / binCount;
        plot(freqRange, 10 * log10(powerSpectrum / txPower));
        axis([min(freqRange) max(freqRange) -100 0]);
        titleText = [ ...
            sprintf('{\\bf Realtime Rx Spectrum Analyzer (%d Hz, %d ms)}', ...
                frequency, durationMs) ...
            sprintf('\nRx Power = %2.2f dB, Rx THD = %3.1f %%', ...
                10 * log10(rxPower / txPower), rxHarmonics / rxPower * 100) ];
        ylabel('Power (dB)');
        xlabel('Frequency (Hz)');
        title(titleText);
        drawnow;
    end

end

function bins = pickMaxBin(fftData, bins)
    % Picks the frequency bin with maximum energy (within a range of 
    % +/- range bins from the specified bins)
    range = 15;

    for i = 1:length(bins)
        check = bins(i) + [-range:range];
        [m, index] = max(fftData(check));
        bins(i) = bins(i) + index - range - 1;
    end
end
