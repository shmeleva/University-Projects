using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    public class InvalidBarcodeException : Exception
    {
        public InvalidBarcodeException(Guid barcode) : base($"Barcode '{barcode.ToString()}' is invalid") { }
    }
}
