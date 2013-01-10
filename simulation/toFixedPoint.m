function a = toFixedPoint(a, totalBits, fractionalBits)
% a = toFixedPoint(a, totalBits, fractionalBits)
%   Converts Matlab array a (in normal floating point representation) to fixed
%   point (integer) representation with totalBits of precision, where the 
%   fractionalBits least significant bits are used to represent the fractional part.

    % Shift to save fractional bits and saturate result
    a = round(a * 2^fractionalBits);
    maxInt = 2^(totalBits - 1) - 1;
    minInt = -2^(totalBits - 1);
    [m, n] = size(a);
    a = min(a, maxInt * ones(m, n));
    a = max(a, minInt * ones(m, n));
end
