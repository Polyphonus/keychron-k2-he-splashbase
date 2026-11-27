# Splash on base

The `keychron` keymap plus two RGB matrix effects that keep every key lit, so
legends stay readable, and send the stock Splash rainbow ripple over it on
each keypress:

- **Splash on base** — rests at the configured color (Fn+E/D hue, Fn+R/F
  saturation); the ripple starts a third of the color wheel away and fades
  back into the base hue.
- **Splash on white** — fixed white base, dimmer at rest, with a dark band
  leading a ripple that saturates quickly to avoid pastel blending.

Both follow the global brightness (Fn+W/S) and speed (Fn+T/G). They come after
Per-key RGB and Mixed RGB in the effect list (light key or Fn+Q/Fn+A).

    make keychron/k2_he/ansi:splashbase
