function generateSpectrum
    global PLOT_PATH;
    global PLOT_EXTENSION;

    % 4-FSK Power Spectrum
    range = -2*pi:0.01:2*pi;

    t1 = sinc(+ range) .^ 2;

    plot(range, 10*log10(t1));
    %semilogy(range, t1);
    axis([min(range) max(range) -40 0]);
    title('{\bfPower Spectrum of Modulated Square Wave}');
    ylabel('Power (dB)');
    xlabel('Normalized Frequency Per Symbol Rate');

    saveas(gcf, [PLOT_PATH 'modulated_power_spectrum' PLOT_EXTENSION]);
end
