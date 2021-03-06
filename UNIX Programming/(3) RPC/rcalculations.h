/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RCALCULATIONS_H_RPCGEN
#define _RCALCULATIONS_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>


#define RCALCULATIONS_PROG ((rpc_uint)3141592)
#define RCALCULATIONS_VERS ((rpc_uint)1)

#ifdef __cplusplus
#define CALCULATE_PI ((rpc_uint)1)
extern "C" double * calculate_pi_1(void *, CLIENT *);
extern "C" double * calculate_pi_1_svc(void *, struct svc_req *);
#define CALCULATE_EXP ((rpc_uint)2)
extern "C" double * calculate_exp_1(double *, CLIENT *);
extern "C" double * calculate_exp_1_svc(double *, struct svc_req *);

#elif __STDC__
#define CALCULATE_PI ((rpc_uint)1)
extern  double * calculate_pi_1(void *, CLIENT *);
extern  double * calculate_pi_1_svc(void *, struct svc_req *);
#define CALCULATE_EXP ((rpc_uint)2)
extern  double * calculate_exp_1(double *, CLIENT *);
extern  double * calculate_exp_1_svc(double *, struct svc_req *);

#else /* Old Style C */
#define CALCULATE_PI ((rpc_uint)1)
extern  double * calculate_pi_1();
extern  double * calculate_pi_1_svc();
#define CALCULATE_EXP ((rpc_uint)2)
extern  double * calculate_exp_1();
extern  double * calculate_exp_1_svc();
#endif /* Old Style C */

#endif /* !_RCALCULATIONS_H_RPCGEN */
