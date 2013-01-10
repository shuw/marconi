function x = bitShift(x, n)
% x = bitShift(x, n)
%   Simulate shift right (n < 0) or shift left (n > 0) on x. Truncation is
%   performed for shift right to simulate effect in 2-s compliment representation.

    if n < 0
        n = -n;
        x = floor(x / 2^n);
    else
        x = x * 2^n;
    end

end

