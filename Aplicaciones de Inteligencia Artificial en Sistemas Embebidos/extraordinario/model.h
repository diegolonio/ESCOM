#ifndef MODEL_H
#define MODEL_H

#define N_CLASES 3
#define N_FEATURES 5
#define VENTANA 50
#define UMBRAL_VALLE 3475L   // umbral del valle de caída

/* media y desviación estándar de cada feature para normalizar los valores */
                                    // media      desv. est.   mínimo      máximo    tiempo en valle
static const float MU[N_FEATURES] = {7932.6678f, 2383.3535f, 5181.4966f, 20360.5944f, 2.4703f};
static const float SD[N_FEATURES] = {1061.6796f, 2836.3707f, 3306.1666f, 15191.8946f, 3.9993f};

//   Clases: 0 = caida, 1 = golpe, 2 = pasivo
static const float W[N_CLASES][N_FEATURES] = {
    // media      desv. est.   mínimo      máximo    tiempo en valle
    {+0.902198f, +1.935802f, -0.140186f, +0.467582f, +2.347043f}, // pasivo
    {-0.643872f, -0.058061f, -1.612086f, +1.613189f, -2.261486f}, // golpe
    {-0.258326f, -1.877741f, +1.752271f, -2.080771f, -0.085557f} // caìda
};

                                //  pasivo       golpe       caída
static const float B[N_CLASES] = {+0.120868f, +1.006492f, -1.127361f}; // sesgo de cada clase

#endif // MODEL_H
