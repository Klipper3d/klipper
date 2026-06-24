import time
import math

class TJC3224_LCD:
    """
    Class representing the control interface for a TJC3224 LCD display.

    This class is based on the T5UIC1_LCD class from the DWIN_T5UIC1_LCD
    repository available on (https://github.com/odwdinc/DWIN_T5UIC1_LCD)
    for the TJC3224_011N display (3.2 inch, 240x320 pixels, no touch) used
    on 3d printers from creality. Most of the instructions are compatible
    with DWIN T5L instruction set available on the manufacturer website
    (https://www.dwin-global.com/uploads/T5L_TA-Instruction-Set-Development-Guide.pdf)
    """

    # Display resolution
    screen_width = 240
    screen_height = 320

    # Data frame structure
    data_frame_head = b"\xAA"
    data_frame_tail = [0xCC, 0x33, 0xC3, 0x3C]
    data_frame = []

    # Font size registers (for unicode and 8 bit text mode)
    font_8x8 = 0x00
    font_6x12 = 0x01
    font_8x16 = 0x02
    font_12x24 = 0x03
    font_16x32 = 0x04
    font_20x40 = 0x05
    font_24x48 = 0x06
    font_28x56 = 0x07
    font_32x64 = 0x08

    # Colors
    color_white = 0xFFFF
    color_black = 0x0000

    # Instructions
    cmd_handshake = 0x00
    cmd_draw_value = 0x14
    cmd_set_palette = 0x40
    cmd_draw_line = 0x51
    cmd_clear_screen = 0x52
    cmd_draw_rectangle = 0x59
    cmd_fill_rectangle = 0x5B
    cmd_reverse_color_area = 0x5C
    cmd_backlight_brightness = 0x5F
    cmd_show_image = 0x70
    cmd_move_screen_area = 0x09
    cmd_draw_icon = 0x97
    cmd_draw_text = 0x98

    # Alias for extra commands
    direction_up = 0x02
    direction_down = 0x03

    def __init__(self, serial):
        """
        Initializes the TJC3224_LCD object.

        Args:
            serial : Serial object to send messages.
        """
        self.serial = serial
    
    def init_display(self):
        print("Sending handshake... ")
        while not self.handshake():
            pass
        print("Handshake response: OK.")

    def byte(self, bool_val):
        """
        Appends a single-byte value to the data frame.

        :param bval: The byte value to be appended.
        :type bval: int
        """
        self.data_frame += int(bool_val).to_bytes(1, byteorder="big")

    def word(self, word_val):
        """
        Appends a two-byte value to the data frame.

        :param wval: The two-byte value to be appended.
        :type wval: int
        """
        self.data_frame += int(word_val).to_bytes(2, byteorder="big")

    def long(self, long_val):
        """
        Appends a four-byte value to the data frame.

        :param lval: The four-byte value to be appended.
        :type lval: int
        """
        self.data_frame += int(long_val).to_bytes(4, byteorder="big", signed=True)

    def double_64(self, double_val):
        """
        Appends an eight-byte value to the data frame.

        :param dval: The eight-byte value to be appended.
        :type value: int
        """
        self.data_frame += int(double_val).to_bytes(8, byteorder="big")

    def string(self, string):
        """
        Appends a UTF-8 encoded string to the data frame.

        :param string: The string to be appended.
        :type string: str
        """
        self.data_frame += string.encode("utf-8")

    def send(self):
        """
        Sends the prepared data frame to the display according to the T5L_TA serial protocol.

        Sends the current contents of the data frame, followed by a predefined
        tail sequence. After sending, the data frame is reset to the head sequence.
        """
        # Write the current data frame and tail sequence to the serial connection
        self.serial.write(self.data_frame)
        self.serial.write(self.data_frame_tail)

        # Reset the data frame to the head sequence for the next transmission
        self.data_frame = self.data_frame_head

        # Delay to allow for proper transmission
        time.sleep(0.001)

    def handshake(self):
        """
        Perform a handshake with the display.

        :return: True if handshake is successful, otherwise False.
        :rtype: bool
        """
        # Send the initiation byte (0x00)
        self.byte(self.cmd_handshake)
        self.send()
        time.sleep(0.1)

        return True

    def set_backlight_brightness(self, brightness):
        """
        Set the backlight luminance.

        :param luminance: Luminance level (0x00-0x40).
        :type luminance: int
        """
        self.byte(self.cmd_backlight_brightness)
        self.byte(min(brightness, 0x40))
        self.send()

    def set_palette(self, background_color=color_black, foreground_color=color_white):
        """
        Set the palette colors for drawing functions.

        :param bg_color: Background color.
        :type bg_color: int
        :param front_color: Foreground (text) color.
        :type front_color: int
        """
        self.byte(self.cmd_set_palette)
        self.word(foreground_color)
        self.word(background_color)
        self.send()

    def clear_screen(self, color=color_black):
        """
        Clear the screen with a specified color.

        :param color: Background color.
        :type color: int
        """
        self.set_palette(color)
        self.byte(self.cmd_clear_screen)
        self.send()

    def draw_point(self, color, x, y):
        """
        Draw a point on the screen.

        :param Color: Color of the point.
        :type Color: int
        :param x: X-coordinate of the point.
        :type x: int
        :param y: Y-coordinate of the point.
        :type y: int
        """
        self.set_palette(self.color_white, color)
        self.byte(self.cmd_draw_line)
        self.word(int(x))
        self.word(int(y))
        self.send()

    def draw_line(self, color, x_start, y_start, x_end, y_end):
        """
        Draw a line segment on the screen.

        :param color: Line segment color.
        :type color: int
        :param x_start: X-coordinate of the starting point.
        :type x_start: int
        :param y_start: Y-coordinate of the starting point.
        :type y_start: int
        :param x_end: X-coordinate of the ending point.
        :type x_end: int
        :param y_end: Y-coordinate of the ending point.
        :type y_end: int
        """
        self.set_palette(self.color_white, color)
        self.byte(self.cmd_draw_line)
        self.word(x_start)
        self.word(y_start)
        self.word(x_end)
        self.word(y_end)
        self.send()

    def draw_rectangle(self, mode, color, x_start, y_start, x_end, y_end):
        """
        Draw a rectangle on the screen.

        :param mode: 0=frame, 1=fill, 2=XOR fill.
        :type mode: int
        :param color: Rectangle color.
        :type color: int
        :param x_start: X-coordinate of the upper-left point.
        :type x_start: int
        :param y_start: Y-coordinate of the upper-left point.
        :type y_start: int
        :param x_end: X-coordinate of the lower-right point.
        :type x_end: int
        :param y_end: Y-coordinate of the lower-right point.
        :type y_end: int
        """
        self.set_palette(self.color_white, color)
        mode_to_command = {
            0: self.cmd_draw_rectangle,
            1: self.cmd_fill_rectangle,
            2: self.cmd_reverse_color_area,
        }
        command = mode_to_command.get(mode, 0)
        self.byte(command)
        self.word(x_start)
        self.word(y_start)
        self.word(x_end)
        self.word(y_end)
        self.send()

    def draw_circle(self, color, x_center, y_center, r):
        """
        Draw a circle on the screen using the draw points method.

        :param Color: Circle color.
        :type Color: int
        :param x_center: X-coordinate of the center of the circle.
        :type x_center: int
        :param y_center: Y-coordinate of the center of the circle.
        :type y_center: int
        :param r: Circle radius.
        :type r: int
        """
        b = 0
        a = 0
        while a <= b:
            b = math.sqrt(r * r - a * a)
            while a == 0:
                b = b - 1
                break
            self.draw_point(
                color, 1, 1, x_center + a, y_center + b
            )  # Draw some sector 1
            self.draw_point(
                color, 1, 1, x_center + b, y_center + a
            )  # Draw some sector 2
            self.draw_point(
                color, 1, 1, x_center + b, y_center - a
            )  # Draw some sector 3
            self.draw_point(
                color, 1, 1, x_center + a, y_center - b
            )  # Draw some sector 4

            self.draw_point(
                color, 1, 1, x_center - a, y_center - b
            )  # Draw some sector 5
            self.draw_point(
                color, 1, 1, x_center - b, y_center - a
            )  # Draw some sector 6
            self.draw_point(
                color, 1, 1, x_center - b, y_center + a
            )  # Draw some sector 7
            self.draw_point(
                color, 1, 1, x_center - a, y_center + b
            )  # Draw some sector 8
            a += 1

    def fill_circle(self, font_color, x_center, y_center, r):
        """
        Fill a circle with a color.

        :param font_color: Fill color.
        :type font_color: int
        :param x_center: X-coordinate of the center of the circle.
        :type x_center: int
        :param y_center: Y-coordinate of the center of the circle.
        :type y_center: int
        :param r: Circle radius.
        :type r: int
        """
        b = 0
        for i in range(r, 0, -1):
            a = 0
            while a <= b:
                b = math.sqrt(i * i - a * a)
                while a == 0:
                    b = b - 1
                    break
                self.draw_point(
                    font_color, 2, 2, x_center + a, y_center + b
                )  # Draw some sector 1
                self.draw_point(
                    font_color, 2, 2, x_center + b, y_center + a
                )  # raw some sector 2
                self.draw_point(
                    font_color, 2, 2, x_center + b, y_center - a
                )  # Draw some sector 3
                self.draw_point(
                    font_color, 2, 2, x_center + a, y_center - b
                )  # Draw some sector 4

                self.draw_point(
                    font_color, 2, 2, x_center - a, y_center - b
                )  # Draw some sector 5
                self.draw_point(
                    font_color, 2, 2, x_center - b, y_center - a
                )  # Draw some sector 6
                self.draw_point(
                    font_color, 2, 2, x_center - b, y_center + a
                )  # Draw some sector 7
                self.draw_point(
                    font_color, 2, 2, x_center - a, y_center + b
                )  # Draw some sector 8
                a = a + 2

    def draw_string(
        self, show_background, size, font_color, background_color, x, y, string
    ):
        """
        Draw a string on the screen.

        :param show_background: True to display the background color, False to not display the background color.
        :type show_background: bool
        :param size: Font size.
        :type size: int
        :param font_color: Character color.
        :type font_color: int
        :param background_color: Background color.
        :type background_color: int
        :param x: X-coordinate of the upper-left point.
        :type x: int
        :param y: Y-coordinate of the upper-left point.
        :type y: int
        :param string: The string to be drawn.
        :type string: str
        """
        self.byte(self.cmd_draw_text)
        self.word(x)
        self.word(y)
        self.byte(0x00)  # font
        self.byte(0x02 | (show_background * 0x40))  # mode (bshow)
        self.byte(size)  # size
        self.word(font_color)
        self.word(background_color)
        self.string(string[:40])
        self.send()

    def draw_string_centered(
        self, show_background, size, font_color, background_color, char_width, char_height, x, y, string
    ):
        """
        Draw a string centered on the screen.

        :param show_background: True to display the background color, False to not display the background color.
        :type show_background: bool
        :param size: Font size.
        :type size: int
        :param font_color: Character color.
        :type font_color: int
        :param background_color: Background color.
        :type background_color: int
        :param char_width: Width of the characters.
        :type char_width: int
        :param char_height: Height of the characters.
        :type char_height: int
        :param x: X-coordinate of the upper-left point.
        :type x: int
        :param y: Y-coordinate of the upper-left point.
        :type y: int
        :param string: The string to be drawn.
        :type string: str
        """
        self.draw_string(
            show_background,
            size,
            font_color,
            background_color,
            int(x - (len(string) * char_width) / 2.0),
            int(y - char_height / 2.0),
            string
        )

    def draw_int_value(
        self,
        show_background,
        zeroFill,
        zeroMode,
        font_size,
        color,
        background_color,
        iNum,
        x,
        y,
        value,
    ):
        """
        Draw a positive integer value on the screen.

        :param show_background: True to display the background color, False to not display the background color.
        :type show_background: bool
        :param zeroFill: True to zero fill, False for no zero fill.
        :type zeroFill: bool
        :param zeroMode: 1 for leading 0 displayed as 0, 0 for leading 0 displayed as a space.
        :type zeroMode: int
        :param font_size: Font size.
        :type font_size: int
        :param color: Character color.
        :type color: int
        :param background_color: Background color.
        :type background_color: int
        :param iNum: Number of digits.
        :type iNum: int
        :param x: X-coordinate of the upper-left point.
        :type x: int
        :param y: Y-coordinate of the upper-left point.
        :type y: int
        :param value: Integer value.
        :type value: int
        """
        self.byte(0x14)
        # Bit 7: bshow
        # Bit 6: 1 = signed; 0 = unsigned number;
        # Bit 5: zeroFill
        # Bit 4: zeroMode
        # Bit 3-0: size
        self.byte(
            (show_background * 0x80)
            | (0 * 0x40)
            | (zeroFill * 0x20)
            | (zeroMode * 0x10)
            | font_size
        )
        self.word(color)
        self.word(background_color)
        self.byte(iNum)
        self.byte(0)  # fNum
        self.word(x)
        self.word(y)
        self.double_64(value)
        self.send()

    def draw_float_value(
        self,
        show_background,
        zeroFill,
        zeroMode,
        size,
        color,
        background_color,
        iNum,
        fNum,
        x,
        y,
        value,
    ):
        """
        Draw a floating point number on the screen.

        :param show_background: True to display the background color, False to not display the background color.
        :type show_background: bool
        :param zeroFill: True to zero fill, False for no zero fill.
        :type zeroFill: bool
        :param zeroMode: 1 for leading 0 displayed as 0, 0 for leading 0 displayed as a space.
        :type zeroMode: int
        :param size: Font size.
        :type size: int
        :param color: Character color.
        :type color: int
        :param background_color: Background color.
        :type background_color: int
        :param iNum: Number of whole digits.
        :type iNum: int
        :param fNum: Number of decimal digits.
        :type fNum: int
        :param x: X-coordinate of the upper-left point.
        :type x: int
        :param y: Y-coordinate of the upper-left point.
        :type y: int
        :param value: Float value.
        :type value: float
        """
        self.byte(self.cmd_draw_value)
        # Bit 7: bshow
        # Bit 6: 1 = signed; 0 = unsigned number;
        # Bit 5: zeroFill
        # Bit 4: zeroMode
        # Bit 3-0: size
        self.byte(
            (show_background * 0x80)
            | (0 * 0x40)
            | (zeroFill * 0x20)
            | (zeroMode * 0x10)
            | size
        )
        self.word(color)
        self.word(background_color)
        self.byte(iNum)
        self.byte(fNum)
        self.word(x)
        self.word(y)
        self.long(value)
        self.send()

    def draw_signed_float(
        self, show_background, size, color, background_color, iNum, fNum, x, y, value
    ):
        """
        Draw a signed floating-point number on the screen.

        :param size: Font size.
        :type size: int
        :param background_color: Background color.
        :type background_color: int
        :param iNum: Number of whole digits.
        :type iNum: int
        :param fNum: Number of decimal digits.
        :type fNum: int
        :param x: X-coordinate of the upper-left corner.
        :type x: int
        :param y: Y-coordinate of the upper-left corner.
        :type y: int
        :param value: Floating-point value to be displayed.
        :type value: float
        """
        if value < 0:
            self.draw_string(
                show_background, size, color, background_color, x - 6, y - 3, "-"
            )
            self.draw_float_value(
                show_background,
                False,
                0,
                size,
                color,
                background_color,
                iNum,
                fNum,
                x,
                y,
                -value,
            )
        else:
            self.draw_string(
                show_background, size, color, background_color, x - 6, y - 3, " "
            )
            self.draw_float_value(
                show_background,
                False,
                0,
                size,
                color,
                background_color,
                iNum,
                fNum,
                x,
                y,
                value,
            )

    def draw_icon(self, show_background, libID, picID, x, y):
        """
        Draw an icon on the screen.

        :param show_background: True to display the background color, False to not display the background color.
        :type show_background: bool
        :param libID: Icon library ID.
        :type libID: int
        :param picID: Icon ID.
        :type picID: int
        :param x: X-coordinate of the upper-left corner.
        :type x: int
        :param y: Y-coordinate of the upper-left corner.
        :type y: int
        """
        if x > self.screen_width - 1:
            x = self.screen_width - 1
        if y > self.screen_height - 1:
            y = self.screen_height - 1
        self.byte(self.cmd_draw_icon)
        self.word(x)
        self.word(y)
        self.byte(libID)
        self.byte(show_background * 0x01)
        self.word(picID)
        self.send()

    def draw_image(self, id=1):
        """
        Draw a JPG image on the screen and cache it in the virtual display area.

        :param id: Picture ID.
        :type id: int
        """
        self.byte(self.cmd_show_image)
        self.byte(id)
        self.send()

    def move_screen_area(
        self, direction, offset, background_color, x_start, y_start, x_end, y_end
    ):
        """
        Copy an area from the virtual display area to the current screen.

        :param direction: Direction ( 0 = ,1 =, 0x02= top, 0x03 = down)
        :type direction: int
        :param offset: How many pixels the copied area is going to be moved.
        :type offset: int
        :param offset: Color of background (to fill the previously moved area?).
        :type offset: int
        :param x_start: X-coordinate of the upper-left corner of the virtual area.
        :type x_start: int
        :param y_start: Y-coordinate of the upper-left corner of the virtual area.
        :type y_start: int
        :param x_end: X-coordinate of the lower-right corner of the virtual area.
        :type x_end: int
        :param y_end: Y-coordinate of the lower-right corner of the virtual area.
        :type y_end: int
        """
        self.byte(self.cmd_move_screen_area)
        self.byte(0x80 | direction)
        self.word(offset)
        self.word(background_color)
        self.word(x_start)
        self.word(y_start)
        self.word(x_end)
        self.word(y_end)
        self.send()
