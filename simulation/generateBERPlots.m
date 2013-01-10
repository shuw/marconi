function generateBERPlots
% generateBERPlots
%   Generates plots of bit error rate versus bit energy per noise power density
%   for 4-FSK and 4-PSK modulation schemes.

    global PLOT_PATH;
    global PLOT_EXTENSION;

    ebno = [5:0.5:15];
    ber4FSK = berawgn(ebno, 'fsk', 4, 'noncoherent');
    ber4PSK = berawgn(ebno, 'psk', 4, 'diff'); 

    semilogy(ebno, ber4FSK, 'r--', ebno, ber4PSK, 'b-.');
    title('{\bfBER vs E_b/N_o for 4-FSK and 4-PSK (AWGN Channel)}');
    legend('4-FSK (orthogonal, noncoherent)', '4-PSK (differentially encoded)');
    xlabel('E_b/N_o (Bit Energy per Noise Power Density, dB)');
    ylabel('BER (Bit Error Rate)');
    grid on;
    saveas(gcf, [PLOT_PATH 'ber_4fsk_4psk' PLOT_EXTENSION]);

    carrierToNoise = 1.3;
    noiseBandwidth = 1006.95;
    baudRate = 100;
    bitsPerSymbol = 2;

    disp(sprintf('Performance with C/N = %.1f dB, Noise BW = %.0f Hz, Baud rate = %.0f', ...
        carrierToNoise, noiseBandwidth, baudRate));

    carrierToNoise = 10 ^ (carrierToNoise/10);
    ebno = (carrierToNoise * noiseBandwidth / (bitsPerSymbol * baudRate));
    ebnoDB = 10 * log10(ebno);

    disp(sprintf(' Eb/No = %.2f (%.2f dB)', ebno, ebnoDB));
    disp(sprintf(' BER for 4-FSK = %d (AWGN)', berawgn(ebnoDB, 'fsk', 4, 'noncoherent')));
    disp(sprintf(' BER for 4-PSK = %d (AWGN)', berawgn(ebnoDB, 'psk', 4, 'diff')));
end
