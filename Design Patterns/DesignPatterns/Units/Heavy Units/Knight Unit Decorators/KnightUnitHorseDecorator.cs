using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Лошадь.
    /// </summary>
    class KnightUnitHorseDecorator : KnightUnitBaseDecorator
    {
        public KnightUnitHorseDecorator(KnightUnit knight) : base(knight) { }

        public override string Name
        {
            get
            {
                return base.Name + " на коне";
            }
        }

        public override Int32 Attack
        {
            get
            {
                return base.Attack + 2;
            }
        }

        public override Int32 Defence
        {
            get
            {
                return base.Defence + 2;
            }
        }

        public override Unit CloneModifiedUnit(Int32 healthPoints)
        {
            return new KnightUnitHorseDecorator((KnightUnit)_knight.CloneModifiedUnit(healthPoints));
        }
    }

}
