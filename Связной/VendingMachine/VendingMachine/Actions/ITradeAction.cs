using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Roles;

namespace VendingMachine.Actions
{
    interface ITradeAction
    {
        string Description { get; }
        Action Execute { get; }
    }
}
