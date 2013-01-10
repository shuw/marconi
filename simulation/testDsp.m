function testDsp(target)
% testDsp
%   Tests the DSP C routines.

    testSignificantBits(target);
    testIirBiquad(target);
    testIntegrateAndDump(target);
    testSqrt(target);
    disp('Testing complete.');

end


function testSignificantBits(target)

    x = 2 .^ [1 : 15] - 1;
    x = [-x x];

    fOut = fopen('test_in.dat', 'wb');
    fwrite(fOut, length(x), 'int32');
    fwrite(fOut, x, 'int16');
    fclose(fOut);

    executeTest('SignificantBits', 'test_in.dat', 'test_out.dat', target);

    fIn = fopen('test_out.dat', 'rb');
    y = fread(fIn, 'uint16')';
    fclose(fIn);

    if ~isequal(y, max(0, floor(log2(abs(x)) + 1)))
        disp('testSignificantBits: FAILED!');
    end

end


function testSqrt(target)

    x = [0 : 2^28 : 2^32];
    x(end) = 2^32 - 1;

    fOut = fopen('test_in.dat', 'wb');
    fwrite(fOut, length(x), 'int32');
    fwrite(fOut, x, 'uint32');
    fclose(fOut);

    executeTest('Sqrt', 'test_in.dat', 'test_out.dat', target);

    fIn = fopen('test_out.dat', 'rb');
    y = fread(fIn, 'uint16')';
    fclose(fIn);

    if y(1) ~= 0 || max(abs(y - sqrt(x))) > 1
        disp('testSqrt: FAILED!');
    end

end


% Binary search for square root
function y = sqrtBinary(x)

    % We exploit the fact that the largest number we deal with is sqrt(2^32 - 1) ~= 65536.
    % Thus divide this search range in half, and perform just enough iteration to 
    % guarantee convergence over all inputs [0 : 2^32 - 1].

    B = 15;
    y = 2^B;
    diff = 2^B;

    if x == 0
        y = 0;
        return;
    end

    for i = 1 : B
        if y^2 < x
            diff = floor(diff / 2);
            y = y + diff;
        elseif y^2 > x
            diff = floor(diff / 2);
            y = y - diff;
        end
    end

end


function testIirBiquad(target)

    [b, a] = butter(2, 0.1);
    x = 0.9 * [ones(1, 64) zeros(1, 64) ones(1, 64)];
    b = round(b * 2^14);
    a = round(a * 2^14);
    x = round(x * 2^15);

    fOut = fopen('test_in.dat', 'wb');
    fwrite(fOut, 14, 'int16');
    fwrite(fOut, b(1), 'int16'); 
    fwrite(fOut, b(2), 'int16'); 
    fwrite(fOut, b(3), 'int16'); 
    fwrite(fOut, a(2), 'int16'); 
    fwrite(fOut, a(3), 'int16'); 
    fwrite(fOut, length(x), 'uint16');
    fwrite(fOut, x, 'int16');
    fclose(fOut);

    executeTest('IirBiquad', 'test_in.dat', 'test_out.dat', target);

    fIn = fopen('test_out.dat', 'rb');
    y = fread(fIn, 'int16')';
    fclose(fIn);

    yRef = biquad(b, a, x, 14);

    if sum(abs(yRef - y)) ~= 0
        disp('testIirBiquad: FAILED!');
    end

end


function y = biquad(b, a, x, shift)

    dx = zeros(1, 2);
    dy = zeros(1, 2);
    y = zeros(1, length(x));

    for i = 1 : length(x)
        y(i) = b(1) * x(i) + b(2) * dx(1) + b(3) * dx(2) - a(2) * dy(1) - a(3) * dy(2);
        y(i) = fix(y(i) / 2^shift);
        dx(2) = dx(1);
        dx(1) = x(i);
        dy(2) = dy(1);
        dy(1) = y(i);
    end

end


function testIntegrateAndDump(target)

    in = randint(1, 32, [-32767 32767]);
    factor = round(1 / 32 * 32768);

    fOut = fopen('test_in.dat', 'wb');
    fwrite(fOut, length(in), 'uint16');
    fwrite(fOut, 15, 'uint16');
    fwrite(fOut, factor, 'int16');
    fwrite(fOut, in, 'int16');
    fclose(fOut);

    executeTest('IntegrateAndDump', 'test_in.dat', 'test_out.dat', target);

    fIn = fopen('test_out.dat', 'rb');
    out = fread(fIn, 'int16');
    fclose(fIn);

    if sum(floor(in * factor / 2^15)) ~= out
        disp('testIntegrateAndDump: FAILED!');
    end

end


function executeTest(testCase, fileIn, fileOut, target)
    % Write all parameters to options file
    fOptions = fopen('test_options.dat', 'wb');
    testCase = padCString(testCase, 32);
    fileIn = padCString(fileIn, 32);
    fileOut = padCString(fileOut, 32);
    fwrite(fOptions, testCase, 'char');
    fwrite(fOptions, fileIn, 'char');
    fwrite(fOptions, fileOut, 'char');
    fclose(fOptions);

    switch target
        case 'sim'
            [s, r] = system([pwd '/../firmware/Marconi/Bin/Sim/DSPTest.exe']);
            if s ~= 0
                disp(r);
                error('Error in runnning DSPTest.exe!');
            end
        case 'hwsim'
            disp('Run TI simulator and press any key when complete...');
            pause;
    end
end
