using System.Collections.Generic;
using VendingMachine.Products;
using VendingMachine.Finance;

namespace VendingMachine.Roles
{
    class Customer : ICustomer
    {
        private readonly IBalance _customerBalance;
        private readonly List<Product> _boughtItems;

        private ISeller seller;
    }
}
