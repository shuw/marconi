function generateDSPTestVectors

    s = [];

    s = [s dspMultVectors];
    s = [s dspConvVectors];
    s = [s dspMagSqVectors];

    % Write header file
    p = pwd;
    fOut = fopen([p '\..\Marconi\Modem\FSKDemodulator\DSPTestVectors.h'], 'wt');
    fwrite(fOut, s);
    fclose(fOut);
end


function s = dspMultVectors
    % DSPMult() test vectors
    x = [-10:10];
    y = [-10:10];
    z = x .* y;
    shift = zeros(1, length(z));

    [o, v] = addTestToList([], [], {x, y, z, shift});
    s = generateTestVectors('dspMultTest', ...
        {'X', 'Y', 'Z', 'Shift'}, {'SWORD', 'SWORD', 'SWORD', 'WORD'}, o, v);
end


function s = dspConvVectors
    
    % DSPConv() test vectors
    precision = 16;
    fixedPoint = 15;
    shift = 15;
    x = [zeros(1, 40) ones(1, 40) zeros(1, 40)];
    x = x .* cos([0:length(x)-1]/8000*2*pi*200);
    x = x * 0.8;
    %x = sin([0:128-1]/8000*2*pi*1);
    %x = -ones(1, 128);
    h = firls(39, [0 0.1 0.1 1], [1 1 0 0]);

    x = toFixedPoint(x, precision, fixedPoint);
    h = toFixedPoint(h, precision, fixedPoint);

    % Simulate fixed-point convolution
    outputCount = length(x) - length(h) + 1;
    hLen = length(h);
    for i = 1:outputCount
        accum = x(i:i+hLen-1) .* h(end:-1:1);
        accum = bitShift(accum, -shift);
        z(i) = sum(accum);
    end
    z = toFixedPoint(z, precision, 0);

    [o, v] = addTestToList([], [], {x, h, z, shift});

    s = generateTestVectors('dspConvTest', ...
        {'X', 'H', 'Z', 'Shift'}, ...
        {'SWORD', 'SWORD', 'SWORD', 'WORD'}, o, v);
end


function s = dspMagSqVectors

    % DSPMagSq test vectors
    precision = 16;
    fixedPoint = 8;
    shift = 8;
    xi = 4 * (1 - 2*rand(1, 128));
    xq = 4 * (1 - 2*rand(1, 128));
    xi = toFixedPoint(xi, precision, fixedPoint);
    xq = toFixedPoint(xq, precision, fixedPoint);

    z = xi.^2 + xq.^2;
    z = bitShift(z, -shift);
    
    [o, v] = addTestToList([], [], {xi, xq, z, shift});
    s = generateTestVectors('dspMagSqTest', ...
        {'Xi', 'Xq', 'Z', 'Shift'}, {'SWORD', 'SWORD', 'SWORD', 'WORD'}, o, v);

end


function [offsets, vectors] = addTestToList(offsets, vectors, newVector)
% Adds a set of test vectors in cell array newVector to the cell array testVector.
% The array newVector{n} is appended to vectors{n} and it's length is appended to
% the appropriate column in offsets.

    if isempty(vectors)
        for i = 1:length(newVector)
            vectors{i} = [];
        end
        offsets = [];
    end

    for i = 1:length(newVector)
        lengths(i) = length(newVector{i}); 
        vectors{i} = [vectors{i} newVector{i}];
    end 
    offsets(end+1,:) = lengths;
end


function s = generateTestVectors(testName, vectorNames, dataTypes, offsets, vectors)
% Generate C code to store test vectors described by offsets and vectors.
    precision = 32;
    numPerLine = 8;

    count = size(offsets);

    s = sprintf('/////////////////////////////////////////////\n');
    s = [s sprintf('// Test vectors for test: %s\n', testName)];
    s = [s sprintf('/////////////////////////////////////////////\n\n')];
    s = [s sprintf('#define %s_COUNT %d\n\n', upper(testName), count(1))];

    for i = 1:length(vectors)
        s = [s sprintf('const WORD %s%sLengths[] = %s;\n\n', testName, vectorNames{i}, ...
            toCStyleArray(offsets(:,i), precision, 0, numPerLine))];
        s = [s sprintf('const %s %s%sVectors[] = %s;\n\n', dataTypes{i}, ...
            testName, vectorNames{i}, ...
            toCStyleArray(vectors{i}, precision, 0, numPerLine))];
    end
end
