function iq = downShift(sampleRate, samples, frequency)
% iq = downShift(sampleRate, samples, frequency)
%   Down shifts the specified samples by the given frequency, resulting in 
%   complex baseband representation (with in-phase and quadrature components).
%   The resulting complex-baseband samples are filtered to remove the
%   doubled frequency components.

    iq = samples .* exp(i * [0:length(samples)-1]/sampleRate * 2*pi * frequency);
    ft = frequency / (sampleRate/2);
    coeff = firls(128, [0 ft ft 1], [1 1 0 0]);
    iq = conv(coeff, iq);
end
