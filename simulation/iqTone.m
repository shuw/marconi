function iqSamples = iqTone(frequency, sampleRate, sampleCount)
% iqSamples = iqTone(frequency, sampleRate, sampleCount)
%   Generates a complex (I,Q) tone of the specified frequency at the given sample
%   rate for the specified number of samples. 

    iqSamples = exp(-j*2*pi*[0:sampleCount-1]/sampleRate*frequency);
end

