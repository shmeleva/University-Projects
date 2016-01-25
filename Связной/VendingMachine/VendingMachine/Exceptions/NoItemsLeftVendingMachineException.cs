using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    public class NoItemsLeftVendingMachineException : VendingMachineException
    {
        public NoItemsLeftVendingMachineException() : base("Выбранный продукт закончился.") { }
    }
}
