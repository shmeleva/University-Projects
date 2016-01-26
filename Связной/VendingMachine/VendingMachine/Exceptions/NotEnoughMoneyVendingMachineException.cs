using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    public class NotEnoughMoneyVendingMachineException : VendingMachineException
    {
        public NotEnoughMoneyVendingMachineException() : base("Внесенной суммы не хватает для покупки") { }
    }
}
