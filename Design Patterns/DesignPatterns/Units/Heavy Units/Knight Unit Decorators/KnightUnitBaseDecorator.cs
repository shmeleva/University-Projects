using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Абстрактный базовый класс для всех декораторов.
    /// </summary>
    abstract class KnightUnitBaseDecorator : KnightUnit
    {
        // Голый рыцарь.
        protected readonly KnightUnit _knight;

        public override String Name
        {
            get
            {
                return _knight.Name;
            }
        }

        public override Int32 Attack
        {
            get
            {
                return _knight.Attack;
            }
        }
        public override Int32 Defence
        {
            get
            {
                return _knight.Defence;
            }
        }
        public override Int32 Cost
        {
            get
            {
                return _knight.Cost;
            }
        }

        public override int Health
        {
            get
            {
                return _knight.Health;
            }
        }

        public override List<Type> AvailableAmmunition
        {
            get
            {
                return _knight.AvailableAmmunition;
            }
        }

        public KnightUnitBaseDecorator(KnightUnit knight)
        {
            this._knight = knight;
        }

        public override UnitKeeper FightBack(int damage)
        {
            KnightUnit changed;
            if (BattleRandom.NextBoolean())
            {
                // Сброс верхней амуниции.
                Type type = this.GetType();
                changed = (KnightUnit)_knight.CloneModifiedUnit(-damage);

                // Возврат амуниции в список доступной.
                changed.AvailableAmmunition.Add(type);
            }
            else
            {
                changed = (KnightUnit)this.CloneModifiedUnit(-damage);
            }

            return new UnitKeeper(this, changed, null);
        }

        public abstract override Unit CloneModifiedUnit(Int32 healthPoints);

        #region IClonableUnit

        public override UnitKeeper MagicallyClone()
        {
            KnightUnit clone = (KnightUnit)this.CloneModifiedUnit(0);
            return new UnitKeeper(this, null, clone);
        }

        #endregion
    }
}
