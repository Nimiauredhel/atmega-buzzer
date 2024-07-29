void sinetest()
{
        OCR0A = E4;
        _delay_ms(500);

        for (int i = 0; i < duration; i ++)
        {
            OCR0A = E4 * sin(wavePos);
            wavePos += direction;
            if (wavePos >= 1.0 || wavePos <= 0.5)
            {
                wavePos -= direction;
                direction *= -1;
            }
        }

        OCR0A = Ab4;
        _delay_ms(500);

        for (int i = 0; i < duration; i ++)
        {
            OCR0A = Ab4 * sin(wavePos);
            wavePos += direction;
            if (wavePos >= 1.0 || wavePos <= 0.5)
            {
                wavePos -= direction;
                direction *= -1;
            }
        }

        OCR0A = B4;
        _delay_ms(500);

        for (int i = 0; i < duration; i ++)
        {
            OCR0A = B4 * sin(wavePos);
            wavePos += direction;
            if (wavePos >= 1.0 || wavePos <= 0.5)
            {
                wavePos -= direction;
                direction *= -1;
            }
        }

        OCR0A = E5;
        _delay_ms(500);

        for (int i = 0; i < duration; i ++)
        {
            OCR0A = E5 * sin(wavePos);
            wavePos += direction;
            if (wavePos >= 1.0 || wavePos <= 0.5)
            {
                wavePos -= direction;
                direction *= -1;
            }
        }
}
