num_samples = 125;
amplitude_value = 100;

theta = linspace(0, pi, num_samples);
sine_values = sin(theta + 0/3 * pi);
mapped_sine = (sine_values + 1) / 2;

fprintf('%d, ', round(sine_values .* amplitude_value));
