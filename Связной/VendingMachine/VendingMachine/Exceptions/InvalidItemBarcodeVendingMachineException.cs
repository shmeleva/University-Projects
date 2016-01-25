using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    public class InvalidItemBarcodeVendingMachineException : VendingMachineException
    {
        public InvalidItemBarcodeVendingMachineException() : base("Выбранный продукт отсутствует в меню.") { }
    }
}
