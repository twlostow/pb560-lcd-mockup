// Extremely ugly code below.
//
// Buckle up before you scroll.



#include <cstdio>
#include <cassert>
#include <sys/time.h>
#include <unistd.h>
#include <map>

#include "HMI_IhmType.hpp"
extern "C"
{
#include "typedef.h"
#include "enum.h"
#include "Structure.h"
#include "DB_Control.h"
#include "DB_Config.h"
#include "DB_Rtc.h"
#include "DB_Current.h"
#include "DB_Compute.h"
#include "DB_PowerSupply.h"
#include "DB_EventMngt.h"
#include "DB_KeyboardEvent.h"
#include "DB_KeyboardEventWrite.h"
#include "DIS_DisplayPage.h"
#include "Driver_Display_Data.h"

#define EEP_VAR_SOURCE
#include "DRV_VarEeprom.h"
}

#include <vector>
#include <deque>
#include <mutex>

#include "fonts/Font.h"
#include "Driver_Display_Data.h"
#include "typedef.h"

#include "sdl_view.h"

#include "Display_Data.h"

#include "../pixmaps/pixmaps.c"

class Timeout
{

public:
    Timeout(uint64_t usec = 50000, bool autoReload = false) : m_timeout(usec), m_autoReload(autoReload), m_active(false){};

    bool expired()
    {
        if (!m_active)
            return false;
        uint64_t tics = getTics();
        uint64_t delta = tics - m_start_us;
        //printf("%p CheckExpired tics %lld delta %lld tmo %lld start %lld\n", this, tics, delta, m_timeout, m_start_us);
        if (delta > m_timeout)
        {
            if (m_autoReload)
            {
                m_start_us = tics;
            }
            else
            {
                m_active = false;
            }

            return true;
        }
        return false;
    }

    void stop()
    {
        m_active = false;
    }

    void restart()
    {
        m_start_us = getTics();
        m_active = true;
    }

    bool isActive() const { return m_active; }

    uint64_t getCountValue()
    {
        uint64_t tics = getTics();
        return tics - m_start_us;
    }

private:
    uint64_t getTics()
    {
        struct timezone tz = {0, 0};
        struct timeval tv;
        gettimeofday(&tv, &tz);
        return (uint64_t)tv.tv_sec * 1000000ULL + tv.tv_usec;
    }

    bool m_autoReload;
    uint64_t m_start_us, m_timeout;
    bool m_active;
};

static std::map<int, Timeout *> g_timers;

extern "C"
{
    extern UWORD16 DB_KeyboardEvent[end_of_db_Keyboard_Event_table];

    UWORD16 DIS_FifoIdxRead;
    UWORD16 DIS_FifoIdxWrite;
    UBYTE DIS_FifoOverflow;
    t_DisplayAccess DIS_FifoDisplay[FIFO_DISPLAY_SIZE];

    void DIS_FifoRead();

    void DB_ControlWrite(e_DB_CONTROL Id,
                         UWORD16 Value);

    UWORD16 DB_ControlRead(e_DB_CONTROL Id);

    UWORD16 DB_ConfigWrite(e_DB_CONFIG Id,
                           UWORD16 Value);

    e_DRV_EVENT_STATE DRV_EVENT_TransStateFlash(void) { return DRV_EVENT_FREE; }

    UBYTE DRV_EVENT_Read_Event(UWORD16 num_event,
                               e_EVENT_PARAMETERS id_event,
                               UWORD16 xhuge *Ptr_retrieval) { return 0; }

    void DB_EventMngt(UWORD16 Value);

void dbOnModeCommand(UWORD16 *Value,
                     e_DATA_OPERATION DataOperation)
{
    static int ventilationMode = PSIMV;

    const e_MODE_TYPES modes[] = { VOL,	PRES,	PSVT,	VSIMV,	PSIMV,	CPAP };

    int inc;

    switch(DataOperation)
    {
        case READ:
            *Value = ventilationMode;
            return;
        case WRITE:
            ventilationMode = *Value;
            return;
        case INCREASE:
            inc = 1;
            break;
        case DECREASE:
            inc = -1;
            break;
    }

    for(int index = 0; index < nb_of_mode; index++ )
    {
        if (modes[index] == ventilationMode)
        {
            index += inc;
            if(index < 0)
                index += nb_of_mode;
            else if (index >= nb_of_mode)
                index -= nb_of_mode;
            ventilationMode = modes[index];
            *Value = ventilationMode;
            return;
        }
    }

}

    UWORD16 DB_IhmAccessParaDataBase(UWORD16 *Value,
                                     UWORD16 Id,
                                     e_TYPE_OF_DB_TARGET DbType,
                                     e_DATA_OPERATION DataOperation)
    {
        
        if ( DbType == KEYBOARD)
        {
            if( DataOperation == READ )
            {
                *Value = DB_KeyboardEvent[Id];
                return TRUE;
            } else if ( DataOperation == WRITE )
            {
                DB_KeyboardEvent[Id] = *Value;
                return TRUE;
            }
        }
          //        printf("AccessParaDB ID %d db %d op %d\n", Id, DbType, DataOperation);
        if( DbType == CONTROL || DbType == ADJUST || DbType == CONFIG )
        {
          
            switch(Id)
            {
                case ADJUST_LANGUAGE_U16:
                    printf("AccessParamDB LANG!\n");
                    *Value = HMI_LANG_ENGLISH; // default lang = EN
                    break;
                case ADJUST_MODE_U16:
                    dbOnModeCommand( Value, DataOperation );
                    break;
                case OFFSET_FAS_VALVE_7_U16:
                case OFFSET_INSP_FLOW_1_U16:
                case OFFSET_INSP_FLOW_2_U16:
                case ADJUST_KEYLOCK_U16:
                case SAVED_MACHINE_COUNTER_MIN_U16:
                case OFFSET_EXH_FLOW_2_U16:
                case VENTIL_REQ_U16:
                    *Value = 0;
                    break;
                	
                default:
                    *Value = 0;
                    printf("ADJUST AccessParaDB ID %d op %d\n", Id, DataOperation);
            }
        }
        //else 
       // {
          //  printf("AccessParaDB ID %d type %d op %d\n", Id, DbType, DataOperation);
           // *Value = 0;
        //}

        return TRUE;
    }

    void It_watchdog(){};

    void EEP_FIFO_PushOneData(UWORD16 xhuge *ptr_data,
                              e_EEP_ACCESS_STATE access_type) {}

    void DRV_Led_Orange(e_TYPE_BLINK) {}

    void DRV_Led_Red(e_TYPE_BLINK) {}

    void DRV_Led_Ventil(e_BOOL) {}

    void DRV_Led_White(e_TYPE_BLINK) {}

    void DRV_Led_Dc(e_TYPE_BLINK) {}

    void DRV_Led_Bat(e_TYPE_BLINK) {}

    void DRV_Led_Ac(e_TYPE_BLINK) {}

    /* Access to the buzzer driver */
    void DRV_Buz_Sound(e_TYPE_SOUND sound,
                       e_NUMBER_BUZ NumBuzzer) {}

    void TIM_StartMinuteDecounter(e_TIM_MINUTE_TABLE_ID offset_table,
                                  UWORD32 value_ms, e_BOOL autoreload)
    {
        g_timers[offset_table] = new Timeout(value_ms * 1000ULL, autoreload);
        g_timers[offset_table]->restart();
    //    printf("TIM_StartMinuteDecounter ID=%d %dmsec autoreload=%d active %d\n", offset_table, value_ms, autoreload, !!g_timers[offset_table]->isActive());
        
    }

    UWORD32 TIM_ReadMinuteDecounterValue(e_TIM_MINUTE_TABLE_ID offset_table)
    {
      //  printf("TIM_ReadMinuteDecounterValue %d\n");
        return g_timers[offset_table]->getCountValue() / 1000ULL;
    }

    void TIM_StopMinuteDecounter(e_TIM_MINUTE_TABLE_ID id)
    {
        //printf("TIM_StopMinuteDecounter %d\n", id);
        g_timers[id]->stop();
    }

    UWORD16 TIM_TestMinuteDecounterActivateOk(e_TIM_MINUTE_TABLE_ID offset_table)
    {
        UWORD16 rv = g_timers[offset_table]->isActive() ? TRUE : FALSE;
        //printf("TIM_TestMinuteDecounterActivateOk %d = %d\n", offset_table, rv);
        return rv;
    }

    UWORD16 TIM_TestMinuteOverflowOk(e_TIM_MINUTE_TABLE_ID offset_table)
    {
        int expired = g_timers[offset_table]->expired() ? 1 : 0;
        
        //if(expired)
          //  printf("TIM_TestMinuteOverflowOk %d expired %d\n", offset_table, expired);
        return expired;
    }

    extern void VENTILATION_HMI_Start(void);
    extern void SETUP_HMI_Start(void);
    extern void CIRCUIT_CHECK_HMI_Start(void);

    void HMI_KeyboardEventLaunch();
    void IHM_TimerEventLauch(void);
    void SYS_VentilationLaunch();
    void SYS_Keyboard();
};

const struct
{
    UWORD16 id;
    const Font *font;
} fontMap[] = {
    {ARIAL9, &font_arial_9},
    {ARIAL8, &font_arial_8},
    {ARIALBLACK11, &font_arial_black_11},
    {ARIALBOLD29, &font_arial_29},
    {0, nullptr}};

class DisplayFifo
{
private:
    std::deque<uint8_t> m_fifo;

public:
    DisplayFifo(){};

    void push(uint8_t d) { m_fifo.push_back(d); }
    bool empty() const { return m_fifo.empty(); }
    uint8_t pop()
    {
        uint8_t d = m_fifo.front();
        m_fifo.pop_front();
        return d;
    }
};

class MonoBitmap
{
public:
    struct PixmapStub
    {
        int x, y, id;
    };

    std::vector<PixmapStub> m_pixmaps;

    MonoBitmap(int w, int h) : m_w(w), m_h(h)
    {
        m_data = new uint8_t[w * h];
        memset(m_data, 0, w * h);
    }

    ~MonoBitmap()
    {
        delete m_data;
    }

    uint8_t getPixel(int x, int y)
    {
        if (x < 0 || y < 0 || x >= m_w || y >= m_h)
            return 0;
        return m_data[m_w * y + x];
    }

    void clearStubs()
    {
        m_pixmaps.clear();
    }

    void drawPixmap(int x0, int y0, int id)
    {
        printf("drawPixmap id %d\n", id );

        for(int i = 0; builtin_pixmaps[i].data; i++)
        {

            if( id == builtin_pixmaps[i].id )
            {
                const uint8_t *data = builtin_pixmaps[i].data;
                int w = data[0];
                int h = data[1];

                data += 2;

                printf("drawPixmap id %d w %d h %d\n", id, w, h );


                for (int y = 0; y < h; y++)
                    for (int x = 0; x < w; x++)
                    {
                        int bit_offset = w * y + x;

                        int v = data[bit_offset >> 3] & ( 1<< ( (bit_offset & 0x7))) ? 1 : 0;

                        setPixel( x0+x, y0+y, v );
                    }

                return;
            }
        }

        PixmapStub stub;

        stub.x = x0;
        stub.y = y0;
        stub.id = id;

        m_pixmaps.push_back(stub);
    }

    void setPixel(int x, int y, uint8_t value, bool negative=false)
    {
        if (x < 0 || y < 0 || x >= m_w || y >= m_h)
            return;
        
        if(!negative)
            m_data[m_w * y + x] = value;
        else
            m_data[m_w * y + x] = !value;
    }

    int drawGlyph(const Font *font, int x0, int y0, char c, bool reverse, bool printon, int base, int lineHeight )
    {
        uint16_t offset = font->offsets[ (uint8_t) c];
        if( offset == 0xffff )
            return 0;

        const uint8_t *buf = font->data + offset;
        const uint8_t *pixmap = buf + 5;

        int w = buf[0];
        int h = buf[1];
        int x_offset = buf[2];
        int y_offset = buf[3];
        int x_advance = buf[4];

        //printf("gadj lh %d base %d diff %d\n", lineHeight, base, );

        int y_adjust = (lineHeight-base);

        if(!printon)
            drawBox(x0, y0 + y_adjust, x_advance, lineHeight, reverse ? 255: 0, false );

        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
            {
                int bit_offset = w * y + x;

                int v = pixmap[bit_offset >> 3] & ( 1<< ( (bit_offset & 0x7))) ? 1 : 0;

                int xp = x + x_offset + x0;
                int yp = y + y_offset + y0 + y_adjust;

                if(v)
                    setPixel( xp, yp, reverse ? !v : v );
            }
        return x_advance;
    }

    void drawText(const Font *font, int x, int y, const char *str, bool reverse, bool printon)
    {
        char c;
        int base = font->data[0];
        int lineHeight = font->data[1];
        while (c = *str++)
            x += drawGlyph(font, x, y, c, reverse, printon, base, lineHeight);
    }

    void drawBox(int x, int y, int w, int h, uint8_t color, bool negative)
    {
        int i, j;

        for (i = 0; i < w; i++)
            for (j = 0; j < h; j++)
                setPixel(x + i, y + j, color, negative);
    }

    void drawLine(int x0, int y0, int x1, int y1, uint8_t color)
    {
         int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    setPixel(x0,y0,color);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
    }

private:
    uint8_t *m_data;
    int m_w, m_h;
};

class HMISimView : public SDLView
{
private:
    int m_currentPage;
    uint8_t m_currentColor;
    bool m_negative;
    Timeout m_refreshTimeout;

    std::vector<MonoBitmap *> m_pages;

    bool m_pageVisible[8];

    const Font *getFont(int index) const
    {
        for (int i = 0; fontMap[i].font; i++)
            if (fontMap[i].id == index)
                return fontMap[i].font;
        return nullptr;
    }

    MonoBitmap *getCurrentPageSurface()
    {
        return m_pages[m_currentPage];
    }

    MonoBitmap *getPageSurface(int page)
    {
        return m_pages[page];
    }

public:
    HMISimView(DisplayFifo *fifo);

    virtual void UserUpdate() override;

private:
    void cmd_displayPage();
    void cmd_negative();
    void cmd_controlPage();
    void cmd_setColor();
    void cmd_putStamp();
    void cmd_setContrast();
    void cmd_backlight();
    void cmd_putString();
    void cmd_eraseScreen();
    void cmd_fillRect();
    void cmd_rect();
    void cmd_line();
    void cmd_pixel();
    virtual void OnKeyEvent( int key, bool up ) override;
    void compositeStuffOnScreen();
    void drawLegend();

    DisplayFifo *m_dispFifo;
};

HMISimView::HMISimView(DisplayFifo *fifo) : SDLView(640, 700), m_dispFifo(fifo)
{
    int i;

    m_refreshTimeout.restart();
    for (i = 0; i < 8; i++)
    {
        m_pages.push_back(new MonoBitmap(320, 240));
        m_pageVisible[i] = false;
    }

    
}

void HMISimView::compositeStuffOnScreen()
{
    if (!m_refreshTimeout.expired())
        return;

    m_refreshTimeout.restart();
    int x, y;

    bool showAllPages = false;

    //m_pages[0]->drawText(&font_arial_black_11, 0, 0, "Hello, world!", false, false );

    //for(int i = 0; i < 5; i++)
      //  m_pageVisible[i] = true;

    for (y = 0; y < 240; y++)
        for (x = 0; x < 320; x++)
        {
            uint8_t r = 0, g = 0, b = 0;
            uint8_t p = 0;
                if (m_pages[0]->getPixel(x, y) && m_pageVisible[0])
                    p |= 128;
                if (m_pages[1]->getPixel(x, y) && m_pageVisible[1])
                    p |= 128;
                if (m_pages[2]->getPixel(x, y) && m_pageVisible[2])
                    p |= 128;
                if (m_pages[3]->getPixel(x, y) && m_pageVisible[3])
                    p |= 64;
                if (m_pages[4]->getPixel(x, y) && m_pageVisible[4])
                    p |= 64;

            if(p)
                r=g=b=255;
            pixelRGBA(m_screen, 2 * x, 2 * y, r, g, b, 255);
            pixelRGBA(m_screen, 2 * x, 2 * y + 1, r, g, b, 255);
            pixelRGBA(m_screen, 2 * x + 1, 2 * y + 1, r, g, b, 255);
            pixelRGBA(m_screen, 2 * x + 1, 2 * y, r, g, b, 255);
        }


    for( int i = 0; i < 5; i++ )
    {
        if( m_pageVisible[i] )
        {
            for( auto st : m_pages[i]-> m_pixmaps)
            {
                char str[16];
                sprintf(str, "%d", st.id);
                rectangleRGBA( m_screen, 2*st.x, 2*st.y, 2*st.x + 20, 2*st.y + 20, 255,128,128,255);
                stringRGBA( m_screen, 2*st.x+4, 2*st.y+4, str, 255, 0, 255, 255 );
            }
        }
    }
}

enum DisplayState
{
    IDLE = 0,
    COMMAND = 1
};

void HMISimView::cmd_displayPage()
{
    uint8_t page = m_dispFifo->pop();

    m_currentPage = page;

    printf("cmd_displayPage %d\n", m_currentPage);
}

void HMISimView::cmd_negative()
{
    uint8_t state = m_dispFifo->pop();
    printf("cmd_negative : %d\n", state);
    m_negative = state;
}

void HMISimView::cmd_setColor()
{
    uint8_t color = m_dispFifo->pop();
    printf("cmd_setColor: %d\n", color);
    m_currentColor = color;
}

void HMISimView::cmd_controlPage()
{
    uint8_t mode = m_dispFifo->pop();
    uint8_t page = m_dispFifo->pop();
    printf("cmd_controlPage: %d %d\n", mode, page);    

    if( mode == PAGEON )
        m_pageVisible[page] = true;
    else
        m_pageVisible[page] = false;
}

void HMISimView::cmd_putStamp()
{
    uint8_t num = m_dispFifo->pop();
    uint8_t posx_msb = m_dispFifo->pop();
    uint8_t posx_lsb = m_dispFifo->pop();
    uint8_t posy = m_dispFifo->pop();

    int x = (posx_msb << 8) | posx_lsb;

    getCurrentPageSurface()->drawPixmap(x, posy, num);
}

void HMISimView::cmd_setContrast()
{
    uint8_t level = m_dispFifo->pop();
    printf("SetContrast: %d\n", level);
}

void HMISimView::cmd_backlight()
{
    uint8_t level = m_dispFifo->pop();
    printf("SetBacklight: %d\n", level);
}

void HMISimView::cmd_putString()
{
    int i;
    char st[99];
    uint8_t printon = m_dispFifo->pop();
    uint8_t font = m_dispFifo->pop();
    uint8_t posx_msb = m_dispFifo->pop();
    uint8_t posx_lsb = m_dispFifo->pop();
    uint8_t posy = m_dispFifo->pop();

    for (i = 0; i < 99; i++)
    {
        char p = m_dispFifo->pop();
        st[i] = p;
        if (p == 0)
            break;
    }

    printf("cmd_putString: page = %d font = %d '%s' printon = %d\n", m_currentPage, font, st, printon);

    auto fnt = getFont( font );

    int x = (posx_msb << 8) | posx_lsb;

    if( !fnt )
        return;

    getCurrentPageSurface()->drawText( fnt, x, posy, st, m_negative, printon );
}

void HMISimView::cmd_eraseScreen()
{
    getCurrentPageSurface()->drawBox(0, 0, 320, 240, 0, false);
    getCurrentPageSurface()->clearStubs();
}

void HMISimView::cmd_fillRect()
{
    uint8_t posx1_msb = m_dispFifo->pop();
    uint8_t posx1_lsb = m_dispFifo->pop();
    uint8_t posy1 = m_dispFifo->pop();
    uint8_t posx2_msb = m_dispFifo->pop();
    uint8_t posx2_lsb = m_dispFifo->pop();
    uint8_t posy2 = m_dispFifo->pop();

    int x1 = (posx1_msb << 8) | posx1_lsb;
    int x2 = (posx2_msb << 8) | posx2_lsb;

    getCurrentPageSurface()->drawBox(
        x1, posy1,
        x2 - x1 + 1, posy2 - posy1 + 1, m_currentColor, false);
}

void HMISimView::cmd_line()
{
    uint8_t posx1_msb = m_dispFifo->pop();
    uint8_t posx1_lsb = m_dispFifo->pop();
    uint8_t posy1 = m_dispFifo->pop();
    uint8_t posx2_msb = m_dispFifo->pop();
    uint8_t posx2_lsb = m_dispFifo->pop();
    uint8_t posy2 = m_dispFifo->pop();

    int x1 = (posx1_msb << 8) | posx1_lsb;
    int x2 = (posx2_msb << 8) | posx2_lsb;

    getCurrentPageSurface()->drawLine(x1, posy1, x2, posy2,  m_negative? !m_currentColor : m_currentColor);
}

void HMISimView::cmd_pixel()
{
    uint8_t posx_msb = m_dispFifo->pop();
    uint8_t posx_lsb = m_dispFifo->pop();
    uint8_t posy = m_dispFifo->pop();

    int x = (posx_msb << 8) | posx_lsb;

    getCurrentPageSurface()->setPixel(x, posy, m_currentColor);
}

void HMISimView::cmd_rect()
{
    uint8_t posx1_msb = m_dispFifo->pop();
    uint8_t posx1_lsb = m_dispFifo->pop();
    uint8_t posy1 = m_dispFifo->pop();
    uint8_t posx2_msb = m_dispFifo->pop();
    uint8_t posx2_lsb = m_dispFifo->pop();
    uint8_t posy2 = m_dispFifo->pop();

    int x1 = (posx1_msb << 8) | posx1_lsb;
    int x2 = (posx2_msb << 8) | posx2_lsb;

    getCurrentPageSurface()->drawLine(x1, posy1, x2, posy1,  m_negative? !m_currentColor : m_currentColor);
    getCurrentPageSurface()->drawLine(x1, posy2, x2, posy2,  m_negative? !m_currentColor : m_currentColor);
    getCurrentPageSurface()->drawLine(x1, posy1, x1, posy2,  m_negative? !m_currentColor : m_currentColor);
    getCurrentPageSurface()->drawLine(x2, posy1, x2, posy2,  m_negative? !m_currentColor : m_currentColor);
}

void HMISimView::UserUpdate()
{
    for (int i = 0; i < 100; i++)
        DIS_FifoRead();

    if (m_dispFifo->empty())
        return;

    uint8_t cmd = m_dispFifo->pop();

    if (cmd == 0x1b)
    {
        cmd = m_dispFifo->pop();

        //printf("cmd: '%c' 0x%x\n", cmd, cmd );
        switch (cmd)
        {
        case 'p':
            cmd_displayPage();
            break;
        case 'i':
            cmd_negative();
            break;
        case 'c':
            cmd_controlPage();
            break;
        case 'C':
            cmd_setColor();
            break;
        case 'I':
            cmd_putStamp();
            break;
        case 'K':
            cmd_setContrast();
            break;
        case 'k':
            cmd_backlight();
            break;
        case 'S':
            cmd_putString();
            break;
        case 'E':
            cmd_eraseScreen();
            break;
        case 'r':
            cmd_fillRect();
            break;
        case 'R':
            cmd_rect();
            break;
        case 'D':
            cmd_line();
            break;
        case 'P':
            cmd_pixel();
            break;
        default:
            printf("unknown cmd '%c'\n", cmd);
            assert(false);
        }
    }

    compositeStuffOnScreen();
    drawLegend();
}

DisplayFifo g_dispFifo;

extern "C" void DRV_DIS_HostRequest(UBYTE x)
{
}

void HMISimView::OnKeyEvent(int key, bool up)
{
    printf("key %d up %d\n", key, !!up);
    
    if(up)
    {
        switch(key)
        {
            case SDLK_UP:
                printf("KEY UP\n"); 
                DB_KeyboardEventWrite(KEYBOARD_UP_EVENT_U16,
                                          TRUE);
               break;
            case SDLK_DOWN:
                printf("KEY DOWN\n"); 
                DB_KeyboardEventWrite(KEYBOARD_DOWN_EVENT_U16,
                                      TRUE);
               break;

            case SDLK_v:
                printf("KEY VALID\n"); 
                DB_KeyboardEventWrite(KEYBOARD_VALID_EVENT_U16,
                                          TRUE);
                break;

            case SDLK_m:
                printf("KEY monit\n"); 
                 DB_KeyboardEventWrite(KEYBOARD_INHIB_EVENT_U16, TRUE);
                break;

            case SDLK_n:
                printf("KEY navig\n"); 
               DB_KeyboardEventWrite(KEYBOARD_NAVIG_EVENT_U16,
                                          TRUE);
                break;

            case SDLK_1:
                printf("KEY start ventillation\n"); 
                DB_KeyboardEventWrite(KEYBOARD_START_VENTIL_DOWN_EVENT_U16,
                                                  TRUE);
                break;
            case SDLK_2:
                printf("KEY stop ventillation\n"); 
                DB_KeyboardEventWrite(KEYBOARD_STOP_VENTIL_EVENT_U16,
                                                TRUE);
                break;
                            
            default:
                break;
        }
    }

}

#define ST_IDLE 0
#define ST_ESC 1
#define ST_COMMAND 2

int state = ST_IDLE;

extern "C" void DRV_DIS_grWriteDisplay(UBYTE x)
{
    //printf("FifoWrite: %x '%c'\n", x, x);

    //if(x == 0xc)
    //for(;;);
    
    //printf("st %d\n", state);

    switch(state)
    {
        case ST_IDLE:
            if(x == 0x1b)
                state = ST_ESC;
            break;
        case ST_ESC:
            if(x=='p')
            state = ST_COMMAND;
            else
            state = ST_IDLE;
            break;
        case ST_COMMAND:
            if( x == 255 )
                assert(false);
            else
                state = ST_IDLE;
            break;
    }
    
    g_dispFifo.push(x);
}

void HMISimView::drawLegend()
{
    lineRGBA( m_screen, 0, 480, 640, 480, 255, 0, 0, 255 );
    stringRGBA( m_screen, 0, 500, "Keyboard mapping: ", 255, 255, 255, 255);
    stringRGBA( m_screen, 0, 515, "- Up/Down arrows: up/down keys", 255, 255, 255, 255);
    stringRGBA( m_screen, 0, 530, "- V             : Valid key", 255, 255, 255, 255);
    stringRGBA( m_screen, 0, 545, "- M             : Monit key", 255, 255, 255, 255);
    stringRGBA( m_screen, 0, 560, "- N             : Navig key", 255, 255, 255, 255);
    stringRGBA( m_screen, 0, 575, "- 1             : Start Ventillation key", 255, 255, 255, 255);
    stringRGBA( m_screen, 0, 590, "- 2             : Stop Ventillation key", 255, 255, 255, 255);
}

int main()
{
    
    HMISimView view(&g_dispFifo);
    for (int i = 0; i < 100; i++)
        g_timers[i] = new Timeout();

    VENTILATION_HMI_Start();
    //for(;;)
    
    
    
    int tics = 0;
    for (;;)
    {
        //SYS_Keyboard();
        HMI_KeyboardEventLaunch();
        IHM_TimerEventLauch();
        if (!view.Update())
            break;
        
    }
    return 0;
}
