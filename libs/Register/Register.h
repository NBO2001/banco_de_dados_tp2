#ifndef REGISTER_H
#define REGISTER_H

class Register {
public:
    int id;
    char title[301];
    unsigned short year;
    char autors[250];
    int cited;
    short year_updated;
    unsigned char month_updated;
    unsigned char day_updated;
    unsigned char hour_updated;
    unsigned char minutes_updated;
    unsigned char seconds_updated;
    char snippet[1025];

    // Register(char* buffer);
    void from_buffer(char* buffer); // Create register from buffer
    void from_bytes(char* buffer);
    char* to_bytes(); // Return bytes array
    void print();
    void static removeSpecialCharacters(unsigned char * string);

};

#endif // REGISTER_H
