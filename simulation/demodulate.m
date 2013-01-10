function bits = demodulate(param, samples)
% bits = demodulate(param, samples)
%   Demodulates the 4-tone FSK samples into bits. Parameters are specified for
%   frequencies, filters, etc.

    p = param;

    phaseHist = [];
    softBitHist = [];
    gainHist = [];
    bits = [];

    % Filters for phase detector and for soft bits
    phaseDelay = zeros(2, max(length(p.PhaseA), length(p.PhaseB)) - 1);
    sbDelay = zeros(2, max(length(p.SoftBitA), length(p.SoftBitB)) - 1);
    amplDelay = zeros(1, max(length(p.AmplB), length(p.AmplB)) - 1);

    filtPhase = zeros(1, 2);
    filtSoftBits = zeros(1, 2);
    amplFilt = 0;
    symbol = zeros(4, 3);
    softBits = zeros(2, 2);
    phaseAccum = 0;
    samplingInstant = 1;

    % Integrate and dump filters for two phases

    while samplingInstant < length(samples) - p.samplesNeeded + 1
        % One symbol of samples for this instant
        s = samples(samplingInstant : samplingInstant + p.samplesNeeded - 1);

        % Measure RMS value of rectified signal and update gain estimate
        rms = mean(abs(s));
        [amplFilt, amplDelay] = filter(p.AmplB, p.AmplA, rms, amplDelay);
        if amplFilt == 0
            gain = inf;
        else
            gain = p.AmplSetLevel / amplFilt;
        end
        gain = max(gain, p.GainMin);
        gain = min(gain, p.GainMax);

        % Downshift
        s1 = s .* exp(-j*2*pi*[0:p.samplesNeeded - 1]/p.sampleRate*p.toneFreq(1));
        s2 = s .* exp(-j*2*pi*[0:p.samplesNeeded - 1]/p.sampleRate*p.toneFreq(2));
        s3 = s .* exp(-j*2*pi*[0:p.samplesNeeded - 1]/p.sampleRate*p.toneFreq(3));
        s4 = s .* exp(-j*2*pi*[0:p.samplesNeeded - 1]/p.sampleRate*p.toneFreq(4));

        % Integrate and dump
        symbol(1, 3:4) = abs([dot(s1, p.FiltPhase1) dot(s1, p.FiltPhase2)]);
        symbol(2, 3:4) = abs([dot(s2, p.FiltPhase1) dot(s2, p.FiltPhase2)]);
        symbol(3, 3:4) = abs([dot(s3, p.FiltPhase1) dot(s3, p.FiltPhase2)]);
        symbol(4, 3:4) = abs([dot(s4, p.FiltPhase1) dot(s4, p.FiltPhase2)]);

        for i = 1 : 2
            % Symbol phase estimate
            [maxAmplitude, maxTone] = max(symbol(:, 1 + i));
            phase = (symbol(maxTone, 2 + i) - symbol(maxTone, i));
            phase = gain * (1 / p.phaseDetectorGain) * p.pllLoopGain * phase;
            [filtPhase(i), phaseDelay(i, :)] = filter(p.PhaseB, p.PhaseA, phase, phaseDelay(i, :)); 

            % Symbol softbits
            t = symbol(:, 1 + i);
            b0 = gain * (t(2) + t(3) - t(1) - t(4));
            b1 = gain * (t(3) + t(4) - t(1) - t(2));

            % Filtered average softbits
            sbAvg = sum(abs([b0 b1])) / 2;
            [filtSoftBits(i), sbDelay(i, :)] = filter(p.SoftBitB, p.SoftBitA, sbAvg, sbDelay(i, :));
            softBits(:, i) = [b0; b1];
        end

        % Pick best symbol phase for phase adjustment and hard bits
        if filtSoftBits(1) > filtSoftBits(2)
            phaseAdjust = filtPhase(1);            
            hb = softBits(:, 1)' > 0;
        else
            phaseAdjust = filtPhase(2);            
            hb = softBits(:, 2)' > 0;
        end
        bits = [bits hb]; 

        phaseAccum = phaseAccum + phaseAdjust;
        phaseAdjust = fix(phaseAccum);
        phaseAccum = phaseAccum - phaseAdjust;
        samplingInstant = samplingInstant + p.symbolLength + phaseAdjust; 

        phaseHist = [phaseHist samplingInstant];
        softBitHist = [softBitHist filtSoftBits'];
        gainHist = [gainHist gain];

        % Save leading symbol edge for next lagging edge
        symbol(:, 1:2) = symbol(:, 3:4);
    end

    if 0
        % Show tracking performance
        phaseHist = phaseHist - (0 : length(phaseHist) - 1) * p.symbolLength;
        plot(phaseHist / max(abs(phaseHist)));
        hold on;
        plot(softBitHist(1, :) - 1, 'b');
        plot(softBitHist(2, :) - 1, 'g');
        plot(gainHist / max(abs(gainHist)) - 2, 'r');
        %hold off;
        %plot(gainHist);
    end

end
