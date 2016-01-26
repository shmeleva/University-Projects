using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    class NoCoinBalanceException : BalanceException
    {
        public NoCoinBalanceException() : base("Отсутствуют монеты с указанным номиналом.") { }
    }
}
