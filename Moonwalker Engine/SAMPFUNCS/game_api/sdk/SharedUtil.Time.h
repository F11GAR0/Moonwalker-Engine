/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        SharedUtil.Time.h
*  PURPOSE:
*  DEVELOPERS:  
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

namespace SharedUtil
{
    //
    // Retrieves the number of milliseconds that have elapsed since the function was first called (plus a little bit to make it look good).
    // This keeps the counter as low as possible to delay any precision or wrap around issues.
    // Note: Return value is module dependent
    //
    uint        GetTickCount32 ( void );

    // Forbid use of GetTickCount
    // #define GetTickCount GetTickCount_has_been_replaced_with_GetTickCount32
    // #define GetTickCount64 GetTickCount64_wont_work_on_XP_you_IDIOT

    //
    // Retrieves the number of milliseconds that have elapsed since some arbitrary point in time.
    //
    // GetTickCount64() exists on Vista and up and is like GetTickCount() except it returns
    // an __int64 and will effectively never wrap. This is an emulated version for XP and down.
    // Note: Wrap around issue is only defeated if the gap between calls is less than 24 days.
    //
    long long   GetTickCount64_ ( void );

    //
    // Retrieves the number of seconds that have elapsed since some arbitrary point in time.
    //
    double      GetSecondCount ( void );

    // Get time in microseconds
    typedef ulong TIMEUS;
    TIMEUS      GetTimeUs ( void );

    // Get tick count cached per module
    long long   GetModuleTickCount64 ( void );
    void        UpdateModuleTickCount64 ( void );

    // Debugging
    void        AddTickCount    ( long long llTickCountAdd );

    //
    // Encapsulate a tick count value
    //
    class CTickCount
    {
        long long m_llTicks;
    public:
        // Constructors
                 CTickCount ( void )                : m_llTicks ( 0 ) {}
        explicit CTickCount ( long long llTicks )   : m_llTicks ( llTicks ) {}
        explicit CTickCount ( double dTicks )       : m_llTicks ( static_cast < long long > ( dTicks ) ) {}

        // Operators
        CTickCount operator+ ( const CTickCount& other ) const  { return CTickCount ( m_llTicks + other.m_llTicks ); }
        CTickCount operator- ( const CTickCount& other ) const  { return CTickCount ( m_llTicks - other.m_llTicks ); }
        CTickCount& operator+= ( const CTickCount& other )      { m_llTicks += other.m_llTicks; return *this; }
        CTickCount& operator-= ( const CTickCount& other )      { m_llTicks -= other.m_llTicks; return *this; }

        // Comparison
        bool operator < ( const CTickCount& other ) const       { return m_llTicks < other.m_llTicks; }
        bool operator > ( const CTickCount& other ) const       { return m_llTicks > other.m_llTicks; }
        bool operator <= ( const CTickCount& other ) const      { return m_llTicks <= other.m_llTicks; }
        bool operator >= ( const CTickCount& other ) const      { return m_llTicks >= other.m_llTicks; }
        bool operator == ( const CTickCount& other ) const      { return m_llTicks == other.m_llTicks; }
        bool operator != ( const CTickCount& other ) const      { return m_llTicks != other.m_llTicks; }

        // Conversion
        double      ToDouble        ( void ) const      { return static_cast < double > ( m_llTicks ); }
        long long   ToLongLong      ( void ) const      { return m_llTicks; }
        int         ToInt           ( void ) const      { return static_cast < int > ( m_llTicks ); }

        // Static functions
        static CTickCount Now ( bool bUseModuleTickCount = false )
        {
            return CTickCount ( bUseModuleTickCount ? GetModuleTickCount64 () : GetTickCount64_ () );
        }
    };

    class CElapsedTime
    {
    public:
        // MaxIncrement should be set higher than the expected tick interval between Get() calls
        CElapsedTime ( void )
        {
            m_llMaxIncrement = INT_MAX;
            m_bUseModuleTickCount = false;
            Reset ();
        }

        void SetMaxIncrement ( long lMaxIncrement, bool bUseModuleTickCount = false )
        {
            m_llMaxIncrement = lMaxIncrement;
            m_bUseModuleTickCount = bUseModuleTickCount;
        }

        void SetUseModuleTickCount ( bool bUseModuleTickCount )
        {
            m_bUseModuleTickCount = bUseModuleTickCount;
        }

        void Reset ( void )
        {
            m_llUpdateTime = DoGetTickCount ();
            m_ullElapsedTime = 0;
        }

        unsigned long long Get ( void )
        {
            long long llTime = DoGetTickCount ();
            m_ullElapsedTime += Clamp ( 0LL, llTime - m_llUpdateTime, m_llMaxIncrement );
            m_llUpdateTime = llTime;
            return m_ullElapsedTime;
        }

    protected:

        long long DoGetTickCount ( void )
        {
            return m_bUseModuleTickCount ? GetModuleTickCount64 () : GetTickCount64_ ();
        }

        long long           m_llUpdateTime;
        unsigned long long  m_ullElapsedTime;
        long long           m_llMaxIncrement;
        bool                m_bUseModuleTickCount;
    };
}
