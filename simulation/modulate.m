function samples = modulate(param, bits)
% samples = modulate(param, bits)
%   Modulates the specified bits using the given modulation parameters.
%   The modulated samples are returned in an array with values [-1,1].

    p = param;
    count = length(bits) / 2;
    bits = reshape(bits, 2, count)';
    samples = [];

    for i = 1:count
        dibit = 2*bits(i,2) + bits(i,1) + 1;
        samples = [samples p.toneSet(:, p.dibitsToSymbol(dibit))'];
    end
end
