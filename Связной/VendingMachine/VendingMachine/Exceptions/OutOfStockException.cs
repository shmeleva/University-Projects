using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VendingMachine.Exceptions
{
    public class OutOfStockException : Exception
    {
        public OutOfStockException(string name) : base($"{name} is out of stock") { }
    }
}
