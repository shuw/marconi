function bits = unpackBits(words, bitsPerWord, bitOrder)
% bits = unpackBits(words, bitsPerWord, bitOrder)
%   Unpacks the words into an array of bits. The number of bits per word is specified as well
%   as the unpacking order ('lsbToMsb' or 'msbToLsb'). Defaults to 16 bits per word and
%   'lsbToMsb' packing.
    if nargin == 1
        bitsPerWord = 16;
        bitOrder = 'lsbToMsb';
    end
    count = length(words);
    switch(bitOrder)
        case 'lsbToMsb'
            factors = 2 .^ [0:bitsPerWord-1];
        case 'msbToLsb'
            factors = 2 .^ [bitsPerWord-1:-1:0];
        otherwise
            error(sprintf('Invalid bit packing specification: %s', bitOrder));
    end
    powers = repmat(factors, count, 1);
    words = repmat(words', 1, bitsPerWord);
    words = floor(words ./ powers);
    words = mod(words, 2);
    bits = reshape(words', 1, count * bitsPerWord);
end
