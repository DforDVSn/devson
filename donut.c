#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

void donut(void)
{
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    char b[1760];

    // Optional: ensure UTF-8/ANSI capable terminal; colors use CSI sequences.
    printf("\x1b[2J");

    for (;;)
    {
        memset(b, 32, 1760);
        memset(z, 0, 7040);
        for (j = 0; j < 6.28; j += 0.07)
        {
            for (i = 0; i < 6.28; i += 0.02)
            {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y = 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
                {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }

         static const char *palette[] = {
             "\x1b[38;5;196m", // '.'  cherry red
             "\x1b[38;5;202m", // ','  orange
             "\x1b[38;5;220m", // '-'  golden yellow
             "\x1b[38;5;118m", // '~'  neon lime
             "\x1b[38;5;82m",  // ':'  green
             "\x1b[38;5;49m",  // ';'  teal
             "\x1b[38;5;45m",  // '='  aqua
             "\x1b[38;5;33m",  // '!'  vivid blue
             "\x1b[38;5;99m",  // '*'  purple
             "\x1b[38;5;201m", // '#'  hot pink
             "\x1b[38;5;208m", // '$'  mango
             "\x1b[38;5;15m"   // '@'  bright white highlight
         };
        /*static const char *palette[] = {
            "\x1b[38;2;46;27;15m",    // #2E1B0F
            "\x1b[38;2;75;46;26m",    // #4B2E1A
            "\x1b[38;2;107;63;43m",   // #6B3F2B
            "\x1b[38;2;139;90;43m",   // #8B5A2B
            "\x1b[38;2;166;107;43m",  // #A66B2B
            "\x1b[38;2;176;122;74m",  // #B07A4A
            "\x1b[38;2;192;127;58m",  // #C07F3A
            "\x1b[38;2;217;166;106m", // #D9A66A
            "\x1b[38;2;239;217;181m", // #EFD9B5
            "\x1b[38;2;255;247;230m", // #FFF7E6
            "\x1b[38;2;255;253;248m", // soft specular
            "\x1b[38;2;255;255;255m"  // bright highlight
        };*/
        /*static const char *palette[] = {
         "\x1b[38;2;31;18;10m", // #1F120A
         "\x1b[38;2;45;28;18m", // deep shadow
         "\x1b[38;2;59;47;47m", // #3B2F2F
         "\x1b[38;2;92;58;33m",
         "\x1b[38;2;123;75;43m",
         "\x1b[38;2;150;90;50m",
         "\x1b[38;2;180;110;60m",
         "\x1b[38;2;210;150;95m",
         "\x1b[38;2;240;200;150m",
         "\x1b[38;2;255;241;201m", // creamy swirl
         "\x1b[38;2;255;250;245m",
         "\x1b[38;2;255;255;255m"
        };
        */
        /*static const char *palette[] = {
            "\x1b[38;2;43;22;13m",
            "\x1b[38;2;60;34;20m",
            "\x1b[38;2;107;63;43m",
            "\x1b[38;2;139;90;43m",
            "\x1b[38;2;170;95;45m",
            "\x1b[38;2;200;120;65m",
            "\x1b[38;2;220;150;90m",
            "\x1b[38;2;235;175;115m",
            "\x1b[38;2;250;200;140m",
            "\x1b[38;2;255;224;168m",
            "\x1b[38;2;255;238;210m",
            "\x1b[38;2;255;255;255m"};*/
            

        static const char *glyphs = ".,-~:;=!*#$@";

        printf("\x1b[H");
        for (k = 0; k < 1761; k++)
        {
            if (k % 80)
            {
                char ch = b[k];
                int idx = 0;
                while (idx < 12 && glyphs[idx] != ch)
                    idx++;
                if (idx < 12)
                {
                    fputs(palette[idx], stdout);
                    putchar(ch);
                    fputs("\x1b[0m", stdout);
                }
                else
                {
                    putchar(ch);
                }
            }
            else
            {
                putchar('\n');
            }
            A += 0.00004;
            B += 0.00002;
        }
        usleep(30000);
    }
}
