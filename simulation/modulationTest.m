function modulationTest(target)

    %keyUp = randint(1, 128, [0, 1]);
    keyUp = randint(1, 8, [0, 1]);
    %sync = randint(1, 128, [0, 1]);
    sync = randint(1, 16, [0, 1]);
    %txBits = randint(1, 1024, [0, 1]);
    txBits = randint(1, 8, [0, 1]);
    padL = zeros(1, 64);
    padR = zeros(1, 64);
    snr = 0;
    shift = 50;

    mode = 'C';
    %mode = 'Matlab';
    
    if isequal(mode, 'C')
        % C version
        input = packBits([keyUp sync txBits padR]);
        %input = packBits([0 0 0 1 1 0 1 1 0 0 0 1 1 0 1 1]);

        modulated = executeModem('ViterbiModulator', input, 'uint16', 'int16', target);
        %modulated = executeModem('Modulator', input, 'uint16', 'int16', target);
        modulated = [zeros(1, 1) modulated zeros(1, 160)];
        %modulated = [zeros(1, shift) modulated zeros(1, 8000)];
        %modulated = 10^(-30/20) * modulated;
        %modulated = awgn(modulated, snr, 'measured');
        output = executeModem('DemodulatorViterbi', modulated, 'int16', 'uint16', target);
        %output = executeModem('Demodulator', modulated, 'int16', 'uint16', target);
        rxBits = unpackBits(output);

        % Convert soft bits to hard bits
        %rxBits = packBits(rxBits, 4, 'lsbToMsb');
        %rxBits = rxBits <= 7;

        % Synchronization
        syncBipolar = 2*sync - 1;
        rxBitsBipolar = 2*rxBits - 1;
        [value offset] = max(xcorr(rxBitsBipolar, syncBipolar));
        offset = length(sync) + offset - length(rxBits) + 1;
        rxBits = rxBits(offset:offset + length(txBits) - 1);

        errorCount = sum(abs(txBits - rxBits)); 
        disp(sprintf('Bit errors = %d', errorCount));
        disp(sprintf('errorrate = %f', errorCount / length(txBits)));

        if 0
        tp = {};
        tp{end + 1}.name = 'gain';
        tp{end}.length = 1;
        tp{end}.type = 'int16';
        tp{end + 1}.name = 'symbolHist';
        tp{end}.length = 16;
        tp{end}.type = 'uint16';
        tp{end + 1}.name = 'phaseFilt';
        tp{end}.length = 2;
        tp{end}.type = 'int16';
        tp{end + 1}.name = 'softBitFilt';
        tp{end}.length = 2;
        tp{end}.type = 'int16';
        tp{end + 1}.name = 'samplingUpdate';
        tp{end}.length = 1;
        tp{end}.type = 'int16';
        p = readTestPoints('test_points.dat', tp);

        % Integrate samplingUpdate
        samplingInstant = cumsum(p.samplingUpdate - 40);

        subplot(5, 1, 1);
        plot(p.symbolHist(:, 9 : 12));
        subplot(5, 1, 2);
        plot(p.symbolHist(:, 13 : 16));
        subplot(5, 1, 3);
        plot(p.gain);
        subplot(5, 1, 4);
        plot(samplingInstant);
        subplot(5, 1, 5);
        %plot(p.phaseFilt);
        plot(p.softBitFilt);
        end

    elseif isequal(mode, 'Matlab')

        p = modulationParameters(8000);

        modulated = modulate(p, [keyUp sync txBits]); 
        modulated = [zeros(1, shift) modulated zeros(1, 20) modulated zeros(1, 8000)];
        modulated = 10 ^ (-40/20) * modulated;
        modulated = awgn(modulated, snr, 'measured');
        rxBits = demodulate(p, modulated); 

        % Synchronization
        syncBipolar = 2*sync - 1;
        rxBitsBipolar = 2*rxBits - 1;
        [value offset] = max(xcorr(rxBitsBipolar, syncBipolar));
        offset = length(sync) + offset - length(rxBits) + 1;
        rxBits = rxBits(offset:offset + length(txBits) - 1);

        errorCount = sum(abs(txBits - rxBits)); 
        disp(sprintf('Bit errors = %d', errorCount));
        disp(sprintf('errorrate = %f', errorCount / length(txBits)));
    end
    
end
