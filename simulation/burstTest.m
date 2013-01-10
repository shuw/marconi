function burstTest(target)
% Tests the BurstTx and BurstRx blocks

    % Check multiple bursts with initial offset
    txText = 'Hello world! How do you do? This is quite fine indeed!!!';
    bits = burstTx(txText, target);
    bits = [ones(1, 8) bits];
    rxText = burstRx(bits, target);

    if(~isequal(txText, rxText))
        disp('FAILED: received text does not match transmitted text');
    end

    % Check burst with error (bad checksum)
    bits = burstTx('Bad data', target);
    bits(50) = 1;
    rxText = burstRx(bits, target);

    if length(rxText) > 0
        disp('FAILED: output burst with bad checksum');
    end

    disp('Testing complete');
end


function bits = burstTx(c, target)
    out = executeModem('BurstTx', c, 'uint16', 'uint16', target);
    bits = unpackBits(out, 16, 'msbToLsb');
end


function c = burstRx(bits, target)
    pad = 16 - mod(length(bits), 16);
    bits = [bits zeros(1, pad) zeros(1, 2*16)];
    in = packBits(bits, 16, 'msbToLsb');
    c = executeModem('BurstRx', in, 'uint16', 'uint16', target);
end
