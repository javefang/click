#ifndef IPINPUTCOMBO_HH
#define IPINPUTCOMBO_HH

/*
 * =c
 * IPInputCombo(COLOR [, BADADDRS])
 * =s IP
 * input combo for IP routing
 * =d
 * A single element encapsulating common tasks on an IP router's input path.
 * Effectively equivalent to
 *
 *   elementclass IPInputCombo { $COLOR, $BADADDRS |
 *     input[0] -> Paint($COLOR)
 *           -> Strip(14)
 *           -> CheckIPHeader($BADADDRS)
 *           -> GetIPAddress(16)
 *           -> [0]output;
 *   }
 *
 * =a Paint, CheckIPHeader, Strip, GetIPAddress, IPOutputCombo
 */

#include <click/element.hh>
#include <click/glue.hh>
#include <click/atomic.hh>

class IPInputCombo : public Element {
  
  u_atomic32_t _drops;
  int _color;

  int _n_bad_src;
  unsigned *_bad_src; // array of illegal IP src addresses.
#if HAVE_FAST_CHECKSUM && FAST_CHECKSUM_ALIGNED
  bool _aligned;
#endif

 public:
  
  IPInputCombo();
  ~IPInputCombo();
  
  const char *class_name() const		{ return "IPInputCombo"; }
  const char *processing() const		{ return AGNOSTIC; }
  
  u_int32_t drops() const			{ return _drops; }
  IPInputCombo *clone() const;
  void add_handlers();
  int configure(const Vector<String> &, ErrorHandler *);

  inline Packet *smaction(Packet *);
  void push(int, Packet *p);
  Packet *pull(int);
  
};

#endif
