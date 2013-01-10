function words = packBits(bits, bitsPerWord, bitOrder)
% words = packBits(bits, bitsPerWord, bitOrder)
%   Packs bits into words with specified number of bits per word. The number of
%   bits per word is specified as well as the packing order ('lsbToMsb' or 'msbToLsb').
%   Defaults to 16 bits per word in 'lsbToMsb' order.
    if nargin == 1
        bitsPerWord = 16;
        bitOrder = 'lsbToMsb';
    end
    rows = length(bits) / bitsPerWord;
    bits = reshape(bits, bitsPerWord, rows)'; 
    switch(bitOrder)
        case 'lsbToMsb'
            factors = 2 .^ [0:bitsPerWord-1];
        case 'msbToLsb'
            factors = 2 .^ [bitsPerWord-1:-1:0];
        otherwise
            error(sprintf('Invalid bit packing specification: %s', bitOrder));
    end
    factors = repmat(factors, rows, 1);
    words = dot(bits, factors, 2)';
end
