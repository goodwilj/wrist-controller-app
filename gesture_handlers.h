// Structs
struct file_descriptors{
    int rd;
    int wr;
    int ex;
    int max;
};

// Core Events
struct file_descriptors create_device();
int destroy_device();

void mouse_left_click();
void mouse_right_click();

void center_cursor();
void move_mouse(int x, int y, int ms);

int get_mouse_coordinates(unsigned char *buf);

// Additional Events
void mouse_hold();
void mouse_release();

void copy();
void paste();

void close_window();


