#pragma once
// Harmony stuff
namespace Tunings {
    typedef enum class intvls { P1, m2, M2, m3, M3, P4, A4, TT, d5, P5, m6, M6, m7, M7 };
    struct Tuning_5lim_sym1 {
        double P1 = 1.0;
        double m2 = 16 / 15.0;
        double M2 = 9 / 8.0;
        double m3 = 6 / 5.0;
        double M3 = 5 / 4.0;
        double P4 = 4 / 3.0;
        double A4 = 45 / 32.0;
        double d5 = 64 / 45.0;
        double P5 = 3 / 2.0;
        double m6 = 8 / 5.0;
        double M6 = 5 / 3.0;
        double m7 = 16 / 9.0;
        double M7 = 15 / 8.0;
    };

    struct Tuning_5lim_sym2 {
        double P1 = 1.0;
        double m2 = 16 / 15.0;
        double M2 = 10 / 9.0;
        double m3 = 6 / 5.0;
        double M3 = 5 / 4.0;
        double P4 = 4 / 3.0;
        double A4 = 45 / 32.0;
        double d5 = 64 / 45.0;
        double P5 = 3 / 2.0;
        double m6 = 8 / 5.0;
        double M6 = 5 / 3.0;
        double m7 = 9 / 5.0;
        double M7 = 15 / 8.0;
    };

    struct Tuning_5lim_asym_std {
        double P1 = 1.0;
        double m2 = 16 / 15.0;
        double M2 = 9 / 8.0;
        double m3 = 6 / 5.0;
        double M3 = 5 / 4.0;
        double P4 = 4 / 3.0;
        double A4 = 45 / 32.0;
        double d5 = 64 / 45.0;
        double P5 = 3 / 2.0;
        double m6 = 8 / 5.0;
        double M6 = 5 / 3.0;
        double m7 = 9 / 5.0;
        double M7 = 15 / 8.0;

    };

    struct Tuning_5lim_asym_ext {
        double P1 = 1.0;
        double m2 = 16 / 15.0;
        double M2 = 9 / 8.0;
        double m3 = 6 / 5.0;
        double M3 = 5 / 4.0;
        double P4 = 4 / 3.0;
        double A4 = 25 / 18.0;
        double d5 = 36 / 25.0;
        double P5 = 3 / 2.0;
        double m6 = 8 / 5.0;
        double M6 = 5 / 3.0;
        double m7 = 9 / 5.0;
        double M7 = 15 / 8.0;

    };

    struct Tuning_7lim {
        double P1 = 1.0;
        double m2 = 15 / 14.0;
        double M2 = 8 / 7.0;
        double m3 = 6 / 5.0;
        double M3 = 5 / 4.0;
        double P4 = 4 / 3.0;
        double A4 = 7 / 5.0;
        double d5 = 10 / 7.0;
        double P5 = 3 / 2.0;
        double m6 = 8 / 5.0;
        double M6 = 5 / 3.0;
        double m7 = 7 / 4.0;
        double M7 = 15 / 8.0;
    };

    struct Tuning_17lim {
        double P1 = 1.0;
        double m2 = 14 / 13.0;
        double M2 = 8 / 7.0;
        double m3 = 6 / 5.0;
        double M3 = 5 / 4.0;
        double P4 = 4 / 3.0;
        double A4 = 17 / 12.0;
        double d5 = 24 / 17.0;
        double P5 = 3 / 2.0;
        double m6 = 8 / 5.0;
        double M6 = 5 / 3.0;
        double m7 = 7 / 4.0;
        double M7 = 13 / 7.0;
    };

    struct Tuning_equal {

    };
}