function param = modulationParameters(sampleRate)
% param = modulationParameters(sampleRate)
%   Configures modulation scheme parameters for use with modulator and demodulator. 

    param.sampleRate = sampleRate;
    param.baudRate = 200; 
    param.symbolLength = param.sampleRate / param.baudRate;
    param.toneFreq = [600 800 1000 1200];

    % Enforce integral number of waveform periods per symbol!
    tonePeriods = param.symbolLength ./ (param.sampleRate ./ param.toneFreq);
    if ~isequal(tonePeriods, round(tonePeriods))
        disp('Tone periods per symbol:');
        disp(tonePeriods);
        error 'Non-integral number of tone periods per symbol';
    end

    param.toneSet = [...
        tone(param.toneFreq(1), param.sampleRate, param.symbolLength);
        tone(param.toneFreq(2), param.sampleRate, param.symbolLength);
        tone(param.toneFreq(3), param.sampleRate, param.symbolLength);
        tone(param.toneFreq(4), param.sampleRate, param.symbolLength)]';

    param.iqToneSet = [...
        iqTone(param.toneFreq(1), param.sampleRate, param.symbolLength);
        iqTone(param.toneFreq(2), param.sampleRate, param.symbolLength);
        iqTone(param.toneFreq(3), param.sampleRate, param.symbolLength);
        iqTone(param.toneFreq(4), param.sampleRate, param.symbolLength)]';

    % Two phases for integrate and dump
    l = param.symbolLength;
    param.FiltPhase1 = [ones(1, l) / l zeros(1, l / 2)];
    param.FiltPhase2 = [zeros(1, l / 2) ones(1, l) / l];
    param.samplesNeeded = 1.5 * l;

    % IIR filters in the receiver
    [param.PhaseB, param.PhaseA] = butter(2, 0.04);
    [param.SoftBitB, param.SoftBitA] = butter(2, 0.07);
    [param.AmplB, param.AmplA] = butter(1, 0.2);

    % AGC parameters
    param.AmplSetLevel = 1/sqrt(2) * 0.5; % RMS of half-scale amplitude
    param.GainMin = 0.25;
    param.GainMax = 100;

    param.dibitsToSymbol = [1 2 4 3];
    param.numSoftBits = 4;

    % Work out gains for PLL: 
    %   phaseDetectorGain is the gain of the detector itself
    %   pllLoopGain is the gain applied to the control loop to produce the desired response
    param.phaseDetectorGain = 1 / 80;
    param.pllLoopGain = 1 / 30;

end
