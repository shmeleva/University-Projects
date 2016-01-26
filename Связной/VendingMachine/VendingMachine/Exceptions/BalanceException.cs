using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    class BalanceException : Exception
    {
        public BalanceException(string message) : base(message) { }
    }
}
