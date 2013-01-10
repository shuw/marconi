function scope
    
    % Standard sampling rate
    fs = 8000;
    %numSamples = fs/100;
    noiseBw = fs/2;

    while 1
        rx = wavrecord(fs, fs, 2);
        noise = rx(:,2);
        noise = sum(noise .^ 2) / length(noise);
        signal = rx(:,1);
        signal = sum(signal .^ 2) / length(signal);
        rx = rx(:,1) + rx(:,2);
        rx = rx .* hanning(length(rx));
        f = abs(fft(rx));
        f = 20*log10(f + 1E-8*ones(length(f), 1));
        freqBins = (0:length(f)-1) * fs/length(f);
        plot(freqBins, f);
        ebno = (1/2) * (signal / noise) * noiseBw / 200;
        title(sprintf('\\bfRealtime Rx Spectrum\nReceived Eb/No = %f', 10*log10(ebno)));
        axis([1 freqBins(round(end*1800/fs)) -50 80]);
        xlabel('Frequency (Hz)');
        ylabel('Amplitude (dB)');
        drawnow;
    end

end
