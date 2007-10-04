#ifndef CLICKY_HVALUES_HH
#define CLICKY_HVALUES_HH 1
#include <click/string.hh>
#include "hashmap1.hh"
#include <gtk/gtk.h>
namespace clicky {
class handler_value;
class handler_values;
class handler_value_iterator;
class wmain;

enum {
    hflag_r = 1 << 0,
    hflag_w = 1 << 1,
    hflag_rparam = 1 << 2,
    hflag_raw = 1 << 3,
    hflag_special = 1 << 4,
    hflag_dead = 1 << 5,
    
    hflag_calm = 1 << 6,
    hflag_expensive = 1 << 7,
    hflag_multiline = 1 << 8,
    hflag_button = 1 << 9,
    hflag_checkbox = 1 << 10,
    hflag_collapse = 1 << 11,
    hflag_visible = 1 << 12,
    hflag_refresh = 1 << 13,
    
    hflag_autorefresh = 1 << 14,
    hflag_autorefresh_outstanding = 1 << 15,
    hflag_have_hvalue = 1 << 16,

    hflag_preferences = 1 << 17,
    hflag_notify_whandlers = 1 << 18,
    hflag_notify_delt = 1 << 19,

    hflag_mandatory_driver_mask = hflag_r | hflag_w | hflag_rparam | hflag_raw
    | hflag_special | hflag_dead,
    hflag_default_driver_mask = hflag_mandatory_driver_mask
    | hflag_calm | hflag_expensive | hflag_multiline
    | hflag_button | hflag_checkbox | hflag_collapse
    | hflag_visible | hflag_refresh
};

class handler_value { public:

    typedef String key_type;

    handler_value(const String &hname)
	: _hname(hname), _flags(0), _driver_flags(0),
	  _driver_mask(hflag_default_driver_mask), _autorefresh_period(1000),
	  _autorefresh_source(0), _next(0) {
	_name_offset = hname.find_right('.');
	_name_offset = (_name_offset < 0 ? 0 : _name_offset + 1);
    }
    ~handler_value() {
	if (_autorefresh_source)
	    g_source_remove(_autorefresh_source);
    }

    const String &hashkey() const {
	return _hname;
    }

    const String &hname() const {
	return _hname;
    }
    String element_name() const {
	if (_name_offset)
	    return _hname.substring(0, _name_offset - 1);
	else
	    return String();
    }
    String handler_name() const {
	return _hname.substring(_name_offset);
    }
    
    const String &hparam() const {
	return _hparam;
    }
    bool have_required_hparam() const {
	return (_flags & (hflag_rparam | hflag_have_hvalue)) != hflag_rparam;
    }
    
    const String &hvalue() const {
	return _hvalue;
    }
    bool have_hvalue() const {
	return (_flags & hflag_have_hvalue) != 0;
    }
    void clear_hvalue() {
	_hvalue = String();
	_flags &= ~hflag_have_hvalue;
    }
    void set_hvalue(const String &hvalue) {
	_hvalue = hvalue;
	_flags |= hflag_have_hvalue;
    }
    
    bool readable() const {
	return (_flags & hflag_r) != 0;
    }
    bool read_param() const {
	return (_flags & hflag_rparam) != 0;
    }
    bool writable() const {
	return (_flags & hflag_w) != 0;
    }
    bool editable() const {
	return (_flags & (hflag_w | hflag_rparam)) != 0;
    }
    bool write_only() const {
	return (_flags & (hflag_r | hflag_w)) == hflag_w;
    }
    bool visible() const {
	return (_flags & hflag_visible) != 0;
    }
    bool refreshable() const {
	return (_flags & hflag_refresh) != 0;
    }
    static bool default_refreshable(int flags) {
	return (flags & (hflag_r | hflag_special | hflag_expensive)) == hflag_r;
    }
    bool special() const {
	return (_flags & hflag_special) != 0;
    }
    bool notify_whandlers() const {
	return (_flags & hflag_notify_whandlers) != 0;
    }
    bool notify_delt() const {
	return (_flags & hflag_notify_delt) != 0;
    }

    int flags() const {
	return _flags;
    }
    int autorefresh_period() const {
	return _autorefresh_period;
    }
    void set_autorefresh_period(int p) {
	_autorefresh_period = p;
    }

    void refresh(wmain *w);
    
    gboolean on_autorefresh(wmain *w, int period);

    void set_driver_flags(wmain *w, int new_flags);

    void set_flags(wmain *w, int new_flags);

    static const String no_hvalue_string;
    
  private:
    
    String _hname;
    int _name_offset;
    String _hparam;
    String _hvalue;
    int _flags;
    int _driver_flags;
    int _driver_mask;
    int _autorefresh_period;
    guint _autorefresh_source;
    handler_value *_next;

    friend class handler_values;
    friend class handler_value_iterator;

    void create_autorefresh(wmain *w);
    
};

struct handler_value_iterator {

    inline void operator++(int) {
	_v = _v->_next;
    }
    inline void operator++() {
	_v = _v->_next;
    }

    inline handler_value *operator->() const {
	return _v;
    }
    inline handler_value &operator*() const {
	return *_v;
    }

  private:

    inline handler_value_iterator(handler_value *v)
	: _v(v) {
    }

    handler_value *_v;

    friend class handler_values;
    friend bool operator==(handler_value_iterator, handler_value_iterator);
    friend bool operator!=(handler_value_iterator, handler_value_iterator);
    
};

inline bool operator==(handler_value_iterator a, handler_value_iterator b) {
    return a._v == b._v;
}

inline bool operator!=(handler_value_iterator a, handler_value_iterator b) {
    return a._v != b._v;
}


struct handler_values {
    
    handler_values(wmain *w);

    typedef handler_value_iterator iterator;
    
    iterator begin(const String &ename);
    
    inline iterator end() {
	return iterator(0);
    }

    handler_value *find(const String &hname) {
	return _hv.find(hname).get();
    }

    handler_value *find_force(const String &hname) {
	return _hv.find_force(hname).get();
    }

    inline bool empty() const {
	return _hv.empty();
    }

    void clear() {
	_hv.clear();
    }
    
    handler_value *set(const String &hname, const String &hparam, const String &hvalue, bool &changed);
    
  private:
    
    wmain *_w;
    HashMap<handler_value> _hv;
    HashMap<String, int> _class_uflags;

    void set_handlers(const String &hname, const String &hparam, const String &hvalue);
    
};

inline void handler_value::set_driver_flags(wmain *w, int new_driver_flags)
{
    _driver_flags = new_driver_flags;
    int new_flags = (_flags & ~_driver_mask) | (_driver_flags & _driver_mask);
    if (new_flags != _flags)
	set_flags(w, new_flags);
}

}
#endif
