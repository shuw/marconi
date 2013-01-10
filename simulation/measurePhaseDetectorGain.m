function measurePhaseDetectorGain(param)
% measurePhaseDetectorGain(param)
%   Plots phase detector output versus sampling offset for the
%   given modulation parameters.

    param = modulationParameters(8000);

    p = param;
    l = p.symbolLength;

    samples = modulate(p, [0 0]);
    samples = [zeros(1, 1*l) samples zeros(1, 1*l)];
    samples = samples / 2;

    % Complex-baseband demodulation
    s1 = samples .* exp(-j*2*pi*[0:length(samples)-1]/p.sampleRate*p.toneFreq(1));
    s2 = samples .* exp(-j*2*pi*[0:length(samples)-1]/p.sampleRate*p.toneFreq(2));
    s3 = samples .* exp(-j*2*pi*[0:length(samples)-1]/p.sampleRate*p.toneFreq(3));
    s4 = samples .* exp(-j*2*pi*[0:length(samples)-1]/p.sampleRate*p.toneFreq(4));

    % Integrate and dump filters
    f = ones(1, p.symbolLength) / p.symbolLength;
    s1 = conv(f, s1);
    s2 = conv(f, s2);
    s3 = conv(f, s3);
    s4 = conv(f, s4);
    s1 = abs(s1);

    phase = s1(1:end-l) - s1(1+l:end);
    
    plot(phase);
    title('Phase Detector Response');
    xlabel('Offset (samples)');
    ylabel('Phase');

end
