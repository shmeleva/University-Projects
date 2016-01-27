using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VendingMachine.Roles;

namespace VendingMachine.Actions
{
    abstract class TradeAction : ITradeAction
    {
        protected readonly ICustomer _customer;
        protected readonly ISeller _seller;

        public abstract string Description { get; }

        public abstract Action Execute { get; }

        protected abstract Action Inform { get; }

        protected abstract Action<ConsoleKeyInfo> Act { get; }

        protected TradeAction(ICustomer customer, ISeller seller)
        {
            ThrowNewArgumentNullException(customer, seller);

            _customer = customer;
            _seller = seller;
        }

        protected void ThrowNewArgumentNullException(ICustomer customer, ISeller seller)
        {
            if (customer == null)
            {
                throw new ArgumentNullException(nameof(customer));
            }

            if (seller == null)
            {
                throw new ArgumentNullException(nameof(seller));
            }
        }

        protected void ThrowNewArgumentOutOfRangeException(int option, int lower, int upper)
        {
            if (InRange(option, lower, upper) == false)
            {
                throw new ArgumentOutOfRangeException(nameof(option));
            }
        }

        protected void ReadKey(Action<ConsoleKeyInfo> major, Action minor, bool autoEsc = true)
        {
            while (true)
            {
                ConsoleKeyInfo keyInfo = Console.ReadKey(true);

                if (keyInfo.Key == ConsoleKey.Escape)
                {
                    break;
                }

                if (major != null)
                {
                    try
                    {
                        major(keyInfo);

                        if (minor != null)
                        {
                            minor();
                            ReadKey(major, minor);
                        }

                        if (autoEsc == false)
                        {
                            ReadKey(null, null);
                        }

                        break;
                    }
                    catch (ArgumentOutOfRangeException)
                    {
                    }
                }
            }
        }

        protected void WriteHeader(int deposit, int wallet)
        {
            Console.Clear();
            Console.WriteLine($"Deposit amount: [{deposit}]\t\tWallet balance: [{wallet}]{Environment.NewLine}");
        }

        protected void WriteEsc()
        {
            Console.WriteLine("[ESC]");
        }

        protected bool InRange(int option, int lower, int upper)
        {
            return (option >= lower) && (option <= upper);
        }
    }
}
