function generateHeaderFile
% generateHeaderFile()
% TODO

    param = modulationParameters(8000);

    headerId = ['__MODULATION_PARAMETERS_H__'];
    dataSize = 16;
    fixedPoint = 15;
    colPerLine = 8;

    % The various lengths and sizes 
    s = [sprintf('#define FSK_SYMBOL_LENGTH           %d\n', param.symbolLength)];
    s = [s sprintf('#define FSK_NUM_TONES               4\n')];
    s = [s sprintf('#define FSK_BITS_PER_SYMBOL         2\n')];
    s = [s sprintf('#define FSK_BITS_PER_SYMBOL_MASK    0x03\n')];

    s = [s sprintf('#define SHIFT_S1_15                 15\n')];
    s = [s sprintf('#define SHIFT_S8_8                  8\n')];

    % Mapping between bit pairs and symbols (subtract 1 since C-arrays 0-indexed)
    s = [s sprintf('static const WORD FSK_DIBITS_TO_SYMBOL[] = {%s};\n', ...
        toCStyleArray(param.dibitsToSymbol - 1, dataSize, 0, '%d', colPerLine))];

    % Complex-baseband signals for demodulation (scale slightly to avoid rail-to-rail signal)
    s = [s sprintf('static const SWORD FSK_VCO_I_SET[FSK_NUM_TONES][FSK_SYMBOL_LENGTH] = {\n')];
    for i = 1:4
        s = [s sprintf('{\n%s\n},\n', ...
            toCStyleArray(0.99 * imag(param.iqToneSet(:,i)), dataSize, fixedPoint, '%6d', colPerLine))];
    end
    s = [s sprintf('};\n')];
    
    s = [s sprintf('static const SWORD FSK_VCO_Q_SET[FSK_NUM_TONES][FSK_SYMBOL_LENGTH] = {\n')];
    for i = 1:4
        s = [s sprintf('{\n%s\n},\n', ...
            toCStyleArray(0.99 * real(param.iqToneSet(:,i)), dataSize, fixedPoint, '%6d', colPerLine))];
    end
    s = [s sprintf('};\n')];

    s = [s sprintf('#define SAMPLES_REQUIRED %d\n', 1.5 * param.symbolLength)];
    s = [s sprintf('#define SYMBOL_LENGTH_FACTOR %d\n', ...
        toFixedPoint(1 / param.symbolLength, 16, 15))];
    s = [s sprintf('#define AMPLITUDE_SET_LEVEL %d\n', ...
        toFixedPoint(param.AmplSetLevel, 16, 15))];
    s = [s sprintf('#define GAIN_MIN %d\n', ...
        toFixedPoint(param.GainMin, 16, 8))];
    s = [s sprintf('#define GAIN_MAX %d\n', ...
        toFixedPoint(param.GainMax, 16, 8))];

    % Signal level filter
    coeff = toFixedPoint([param.AmplB(1 : 2) 0 param.AmplA(2) 0], 16, 8);  
    s = [s sprintf('const SWORD LEVEL_FILT[] = {\n%s\n};\n', ...
        toCStyleArray([coeff 8 zeros(1, 4)], 16, 0, '%6d', 10))];

    % Phase detector filter
    coeff = toFixedPoint([param.PhaseB(1 : 3) param.PhaseA(2 : 3)], 16, 8);  
    s = [s sprintf('const SWORD PHASE_FILT[] = {\n%s\n};\n', ...
        toCStyleArray([coeff 8 zeros(1, 4)], 16, 0, '%6d', 10))];

    % Softbit amplitude filter
    coeff = toFixedPoint([param.SoftBitB(1 : 3) param.SoftBitA(2 : 3)], 16, 8);  
    s = [s sprintf('const SWORD SOFT_BIT_FILT[] = {\n%s\n};\n', ...
        toCStyleArray([coeff 8 zeros(1, 4)], 16, 0, '%6d', 10))];

    s = [s sprintf('#define PLL_GAIN_FACTOR %d\n', ...
        toFixedPoint((1 / param.phaseDetectorGain) * param.pllLoopGain, 16, 8))];
   




    disp(s);
end
