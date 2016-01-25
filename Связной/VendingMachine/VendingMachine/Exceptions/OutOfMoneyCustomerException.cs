using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    class OutOfMoneyCustomerException : CustomerException
    {
        public OutOfMoneyCustomerException() : base("Кончились деньги.") { }
    }
}
