/**
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <sx/command/ec-multiply-secrets.hpp>

#include <iostream>
#include <bitcoin/bitcoin.hpp>
#include <sx/serializer/secret.hpp>
#include <sx/utility/coin.hpp>
#include <sx/utility/console.hpp>

using namespace bc;
using namespace sx;
using namespace sx::extension;
using namespace sx::serializer;

// 100% coverage by line, loc ready.
console_result ec_multiply_secrets::invoke(std::istream& input, 
    std::ostream& output, std::ostream& cerr)
{
    // Bound parameters.
    auto secrets = get_secrets_argument();

    bool first = true;
    secret product(secrets[0]);
    for (auto const& secret: secrets)
    {
        if (first)
        {
            first = false;
            continue;
        }

        // Elliptic curve function (INTEGER * INTEGER) % curve-order.
        if (!bc::ec_multiply(product.data(), secret))
        {
            cerr << SX_EC_MULITPLY_SECRETS_OUT_OF_RANGE << std::endl;
            return console_result::failure;
        }
    }

    output << product << std::endl;
    return console_result::okay;
}
