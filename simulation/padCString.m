function s = padCString(s, maxLen)
    s = s(1 : min(maxLen - 1, length(s)));
    s = [s zeros(1, maxLen - mod(length(s), maxLen))];
end
