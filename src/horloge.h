// --------------------
// digital hour to word clock display
// --------------------


#define SizeOfTab(x)  (sizeof(x) / sizeof (x[0]))


// for each hour return the list of symbols to display 
struct s_Word {
       byte   pos;
       byte   len;
};

struct s_HourCode {
     struct s_Word word1;
     struct s_Word word2;
};


struct s_MinuteCode {
     struct s_Word  word1;
     struct s_Word  word2;
     struct s_Word  word3;
};


union u_Words {
    struct 
    {
      struct s_HourCode   h;
      struct s_MinuteCode m;
    } s;                              // use by horloge.c
    struct s_Word  words[5];   // mapping for display routines
};

void init_Horloge();
union u_Words *disp_Horloge (int hours, int minutes, union u_Words *data);
