using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Шлем.
    /// </summary>
    class KnightUnitHelmetDecorator : KnightUnitBaseDecorator
    {
        public KnightUnitHelmetDecorator(KnightUnit knight) : base(knight) { }

        public override string Name
        {
            get
            {
                return base.Name + " в шлеме";
            }
        }

        public override Int32 Defence
        {
            get
            {
                return base.Defence + 1;
            }
        }

        public override Unit CloneModifiedUnit(Int32 healthPoints)
        {
            return new KnightUnitHelmetDecorator((KnightUnit)_knight.CloneModifiedUnit(healthPoints));
        }
    }

}
