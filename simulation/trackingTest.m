function trackingTest

    p = modulationParameters(8000);

    for i = 1:10
    keyUp = [0 0 0 1 1 1 1 0 0 0 0 1 1 1 1 0];
    sync = randint(1, 16, [0 1]);
    txBits = randint(1, 512, [0 1]);
    % + 40 seems to be about 1/2 symbol offset here
    space = zeros(1, 40*4 + 40);
    s = [space modulate(p, [keyUp sync txBits]) space];
    %s = awgn(s, 3, 'measured');
    %wavplay(s);

    rxBits = demodulate(p, s);

    % Synchronization
    syncBipolar = 2*sync - 1;
    rxBitsBipolar = 2*rxBits - 1;
    [value offset] = max(xcorr(rxBitsBipolar, syncBipolar));
    offset = length(sync) + offset - length(rxBits) + 1;
    rxBits = rxBits(offset:offset + length(txBits) - 1);

    errorCount = sum(abs(txBits - rxBits)); 
    disp(sprintf('Bit errors = %d', errorCount));

    if(errorCount > 0)
        pause;
    end
    end


end
