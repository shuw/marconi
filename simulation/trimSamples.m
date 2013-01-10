function samples = trimSamples(samples, width)
% samples = trimSamples(samples, width)
%   Trims each end of a vector of samples. An equal amount is trimmed from each 
%   end, and the specified width is the number of samples to preserve. 

    begin = round(length(samples) / 2 - (width / 2));
    samples = samples(begin:begin+width-1);
end
