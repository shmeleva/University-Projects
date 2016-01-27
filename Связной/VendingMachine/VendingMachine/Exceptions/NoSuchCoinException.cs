using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    class NoSuchCoinException : Exception
    {
        public NoSuchCoinException() : base("No coin(s) of such value") { }
    }
}
