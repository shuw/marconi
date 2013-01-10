function s = toCStyleArray(a, totalBits, fractionalBits, format, elementsPerLine)
% s = toCStyleArray(a, totalBits, fractionalBits, format, elementsPerLine)
%   Convert Matlab array data to nicely formatted C-style array.  The array a is
%   represented as a signed integer with totalBits of precision, including
%   fractionalBits of fixed-point precision. The number of array elements output
%   per line is given in elementsPerLine; the sprintf(...) format string for
%   each element is also specified. C array is returned as a string in s.

    a = toFixedPoint(a, totalBits, fractionalBits);

    s = [];
    format = [format ', '];
    for i = 1:elementsPerLine:length(a)
        last = min(i + elementsPerLine - 1, length(a));
        s = [s sprintf(format, a(i:last))];
        if last ~= length(a)
            s = [s sprintf('\n')];
        end
    end
end
