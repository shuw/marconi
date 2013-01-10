function samples = tone(frequency, sampleRate, sampleCount)
% samples = tone(frequency, sampleRate, sampleCount)
%   Generates a sinusoidal tone of the specified frequency at the given sample
%   rate for the specified number of samples. 

    samples = sin([0:sampleCount - 1] / sampleRate * 2*pi * frequency);
    %samples = cos([0:sampleCount - 1] / sampleRate * 2*pi * frequency);
end


