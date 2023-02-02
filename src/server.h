
static char x_old_red[4], y_old_red[4], x_new_red[4], y_new_red[4];
static char x_old_black[4], y_old_black[4], x_new_black[4], y_new_black[4];
static int which_to_receive_red = 0; // 0放在x_new里，1放在y_new里
static int which_to_receive_black = 0;
static int start = 0;
static int change = 0;
static char starting[4] = "ooo";
static int whose_turn = 0; // 0为红方，默认为client0，1为黑方，为client1
